#include "synth_spi.h"

#include <synth/midi/midi_control.h>

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
  uint power_led;
};

Config const config{

    .synth_spi =
        {.mosi = 15, .miso = 12, .clock = 14, .chip_select = 13, .spi = spi1},

    .power_led = 22

};

SynthSpi synth_spi{config.synth_spi};

MidiControl midi{synth_spi};

queue_t midi_messages{};

namespace core_0 {

void midi_task() {
  while (tud_midi_available()) {
    std::array<uint8_t, 4> packet{};
    tud_midi_packet_read(packet.data());
    queue_add_blocking(&midi_messages, packet.data());
  }
}

void task() {
  for (;;) {
    tud_task();
    midi_task();
  }
}

} // namespace core_0

namespace core_1 {

void task() {
  synth_spi.init();

  for (;;) {
    std::array<uint8_t, 4> packet{};

    while (queue_try_remove(&midi_messages, &packet)) {
      midi.handle(packet);
    }
  }
}

} // namespace core_1

} // namespace

int main() {
  bi_decl(bi_4pins_with_func(
      config.synth_spi.miso, config.synth_spi.chip_select,
      config.synth_spi.clock, config.synth_spi.mosi, GPIO_FUNC_SPI));
  bi_decl(bi_1pin_with_name(config.power_led, "LED"));

  gpio_init(config.power_led);
  gpio_set_dir(config.power_led, GPIO_OUT);
  gpio_put(config.power_led, true);

  board_init();

  tud_init(BOARD_TUD_RHPORT);

  queue_init(&midi_messages, 4 * sizeof(uint8_t), 16);

  multicore_launch_core1(core_1::task);

  core_0::task();
}
