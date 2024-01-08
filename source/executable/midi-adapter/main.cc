#include "hw_config.h"

#include "synth.h"
#include "ui/ui.h"
#include "ui/wave_form_selection.h"

#include <display.h>
#include <gpio_interrupt_handler.h>
#include <push_button.h>
#include <rotary_encoder.h>

#include <lvgl.h>

#include <bsp/board.h>
#include <src/font/lv_symbol_def.h>
#include <src/misc/lv_area.h>
#include <tusb.h>

#include <hardware/gpio.h>
#include <hardware/timer.h>
#include <pico/binary_info.h>
#include <pico/multicore.h>
#include <pico/util/queue.h>

namespace {

Config const config{

    .display = {.clock = 18,
                .mosi = 19,
                .chip_select = 17,
                .data_command = 20,
                .reset = 21,
                .spi = spi0},

    .synth =
        {.mosi = 15, .miso = 12, .clock = 14, .chip_select = 13, .spi = spi1},

};

Synth synth{config.synth};

lv::Display display{config.display};

GpioInterruptHandler gpioIrqHandler{};
RotaryEncoder select{{.gpio_a = 0, .gpio_b = 1}};
PushButton confirm{{.gpio = 2}};

UI ui{};

queue_t enc_value{};

queue_t packets{};

void midi_task() {
  while (tud_midi_available()) {
    std::array<uint8_t, 4> packet{};
    tud_midi_packet_read(packet.data());

    const uint32_t data =
        packet[0] | packet[1] << 8 | packet[2] << 16 | packet[3] << 24;
    queue_add_blocking(&packets, &data);
  }
}

void second_core_entry() {
  synth.init();

  gpioIrqHandler.init();
  select.init(gpioIrqHandler);
  confirm.init(gpioIrqHandler);

  auto last = time_us_64();
  int value = 0;
  queue_add_blocking(&enc_value, &value);

  for (;;) {
    uint32_t packet{};

    while (queue_try_remove(&packets, &packet)) {
      synth.handle(packet);
    }

    gpioIrqHandler.task();

    auto const now = time_us_64();
    if (now - last > 100'000ull) {
      last = now;

      if (select.changed()) {
        value += select.pop_delta();
        queue_try_add(&enc_value, &value);
      }

      if (confirm.pop_pushed()) {
        value = 0;
        queue_try_add(&enc_value, &value);
      }
    }
  }
}

} // namespace

int main() {
  bi_decl(bi_3pins_with_func(17, 18, 19, GPIO_FUNC_SPI));
  bi_decl(bi_2pins_with_names(20, "D/C", 21, "RST"));
  bi_decl(bi_4pins_with_func(12, 13, 14, 15, GPIO_FUNC_SPI));
  bi_decl(bi_1pin_with_name(22, "LED"));

  gpio_init(22);
  gpio_set_dir(22, GPIO_OUT);
  gpio_put(22, true);

  board_init();
  lv_init();

  tud_init(BOARD_TUD_RHPORT);

  display.initialize();

  queue_init(&enc_value, sizeof(int), 2);
  queue_init(&packets, sizeof(uint32_t), 16);

  ui.show();

  multicore_launch_core1(second_core_entry);

  for (;;) {
    tud_task();
    midi_task();

    int value = -1;
    if (queue_try_remove(&enc_value, &value)) {
      auto const osc = ((value / 5) % 4) + 1;
      ui.select_oscillator(osc);

      switch (value % 4) {
      default:
        ui.select_wave_form(WaveForm::Square);
        break;

      case 1:
        ui.select_wave_form(WaveForm::Sawtooth);
        break;

      case 2:
        ui.select_wave_form(WaveForm::Triangle);
        break;

      case 3:
        ui.select_wave_form(WaveForm::Noise);
        break;
      }
    }
  }
}
