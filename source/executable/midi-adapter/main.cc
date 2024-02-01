#include "hw_config.h"

#include "controller.h"
#include "knob.h"
#include "synth.h"
#include "ui/ui.h"
#include "ui/wave_form_selection.h"

#include <display.h>
#include <gpio_interrupt_handler.h>
#include <push_button.h>
#include <rotary_encoder.h>

#include <lvgl.h>

#include <bsp/board.h>
#include <tusb.h>

#include <hardware/gpio.h>
#include <hardware/timer.h>
#include <pico/binary_info.h>
#include <pico/multicore.h>
#include <pico/util/queue.h>

namespace {

Config const hw_config{

    .display = {.clock = 18,
                .mosi = 19,
                .chip_select = 17,
                .data_command = 20,
                .reset = 21,
                .spi = spi0},

    .synth =
        {.mosi = 15, .miso = 12, .clock = 14, .chip_select = 13, .spi = spi1},

    .select = {.gpio_a = 0, .gpio_b = 1},
    .confirm = {.gpio = 2},

    .power_led = 22

};

constexpr std::initializer_list<int> const oscillator_ids{1, 2, 3, 4};

Synth synth{hw_config.synth};

lv::Display display{hw_config.display};

GpioInterruptHandler gpioIrqHandler{};
RotaryEncoder select{hw_config.select};
PushButton confirm{hw_config.confirm};
Knob knob{select, confirm};

UI ui{};

Controller controller{knob, ui, synth, oscillator_ids};

queue_t packets{};

// ----- Core 0 -----

void midi_task() {
  while (tud_midi_available()) {
    std::array<uint8_t, 4> packet{};
    tud_midi_packet_read(packet.data());

    const uint32_t data =
        packet[0] | packet[1] << 8 | packet[2] << 16 | packet[3] << 24;
    queue_add_blocking(&packets, &data);
  }
}

void task_core0() {
  controller.init();

  for (;;) {
    tud_task();
    midi_task();

    controller.task();
  }
}

// ----- Core 1 -----

void task_core1() {
  synth.init();

  gpioIrqHandler.init();
  select.init(gpioIrqHandler);
  confirm.init(gpioIrqHandler);
  knob.init();

  for (;;) {
    uint32_t packet{};

    while (queue_try_remove(&packets, &packet)) {
      synth.handle(packet);
    }

    gpioIrqHandler.task();
    knob.task();
  }
}

} // namespace

int main() {
  bi_decl(bi_3pins_with_func(17, 18, 19, GPIO_FUNC_SPI));
  bi_decl(bi_2pins_with_names(20, "D/C", 21, "RST"));
  bi_decl(bi_4pins_with_func(12, 13, 14, 15, GPIO_FUNC_SPI));
  bi_decl(bi_1pin_with_name(22, "LED"));
  bi_decl(bi_3pins_with_names(0, "A", 1, "B", 2, "S"));

  gpio_init(hw_config.power_led);
  gpio_set_dir(hw_config.power_led, GPIO_OUT);
  gpio_put(hw_config.power_led, true);

  board_init();
  lv_init();

  tud_init(BOARD_TUD_RHPORT);

  display.initialize();

  queue_init(&packets, sizeof(uint32_t), 16);

  ui.show();

  multicore_launch_core1(task_core1);

  task_core0();
}
