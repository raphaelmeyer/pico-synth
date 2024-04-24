#include "synth_spi.h"

#include <synth/control/control.h>
#include <synth/control/focus.h>
#include <synth/control/midi_control.h>
#include <synth/control/model.h>
#include <synth/control/synth_control.h>

#include <synth/io/gpio_irq.h>
#include <synth/io/push_button.h>
#include <synth/io/rotary_encoder.h>

#include <synth/device/display.h>
#include <synth/device/st7735.h>

#include <synth/ui/ui.h>

#include <bsp/board.h>
#include <tusb.h>

#include <pico/binary_info.h>
#include <pico/multicore.h>
#include <pico/util/queue.h>

#include <hardware/gpio.h>

#include <array>
#include <cstdint>

namespace {

struct Config {
  SynthSpiConfig synth_spi;

  RotaryEncoderConfig select;
  PushButtonConfig confirm;

  St7735Config lcd;

  uint power_led;
};

Config const config{

    .synth_spi =
        {.mosi = 15, .miso = 12, .clock = 14, .chip_select = 13, .spi = spi1},

    .select = {.gpio_a = 0, .gpio_b = 1},
    .confirm = {.gpio = 2},

    .lcd = {.clock = 18,
            .mosi = 19,
            .chip_select = 17,
            .data_command = 20,
            .reset = 21,
            .spi = spi0},

    .power_led = 22

};

queue_t midi_messages{};
queue_t control_events{};

Model model{};
Focus focus{};

UI ui{model, focus};

SynthSpi synth_spi{config.synth_spi};
MidiControl midi{synth_spi};
SynthControl synth{model, focus, synth_spi};

Control control{model, focus, [](ControlEvent event) {
                  synth.handle(event);
                  queue_add_blocking(&control_events, &event);
                }};

GpioIrq gpio{};

RotaryEncoder select{config.select,
                     [](int steps) { control.handle(Rotate{steps}); }};

PushButton confirm{config.confirm, [] { control.handle(Click{}); }};

St7735 lcd{config.lcd};
Display display{lcd};

using UsbMidiPacket = std::array<uint8_t, 4>;

namespace core_0 {

void midi_task() {
  while (tud_midi_available()) {
    UsbMidiPacket packet{};
    tud_midi_packet_read(packet.data());
    queue_add_blocking(&midi_messages, packet.data());
  }
}

void task() {
  for (;;) {
    tud_task();
    midi_task();

    ControlEvent event{};
    while (queue_try_remove(&control_events, &event)) {
      ui.handle(event);
    }
  }
}

} // namespace core_0

namespace core_1 {

void task() {
  gpio.init();
  select.init(gpio);
  confirm.init(gpio);
  synth_spi.init();

  for (;;) {
    UsbMidiPacket packet{};

    while (queue_try_remove(&midi_messages, &packet)) {
      midi.handle(packet);
    }

    gpio.task();
    select.task();
  }
}

} // namespace core_1

} // namespace

int main() {
  bi_decl(bi_4pins_with_func(
      config.synth_spi.miso, config.synth_spi.chip_select,
      config.synth_spi.clock, config.synth_spi.mosi, GPIO_FUNC_SPI));
  bi_decl(bi_3pins_with_names(config.select.gpio_a, "A", config.select.gpio_b,
                              "B", config.confirm.gpio, "S"));
  bi_decl(bi_1pin_with_name(config.power_led, "LED"));
  bi_decl(bi_3pins_with_func(config.lcd.chip_select, config.lcd.clock,
                             config.lcd.mosi, GPIO_FUNC_SPI));
  bi_decl(bi_2pins_with_names(config.lcd.data_command, "D/C", config.lcd.reset,
                              "RST"));

  gpio_init(config.power_led);
  gpio_set_dir(config.power_led, GPIO_OUT);
  gpio_put(config.power_led, true);

  board_init();

  lcd.init();
  lv_init();
  display.init();

  tud_init(BOARD_TUD_RHPORT);

  queue_init(&midi_messages, 4 * sizeof(uint8_t), 16);
  queue_init(&control_events, sizeof(ControlEvent), 4);

  multicore_launch_core1(core_1::task);

  core_0::task();
}
