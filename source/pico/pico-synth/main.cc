#include "adapter_spi.config.h"
#include "adapter_spi.h"

#include <synth/device/i2s.config.h>
#include <synth/device/i2s.h>
#include <synth/device/pico-random.h>
#include <synth/message/message.h>
#include <synth/synth/synth.h>

#include <pico/binary_info.h>
#include <pico/multicore.h>
#include <pico/util/queue.h>

#include <hardware/gpio.h>

namespace {

constexpr uint32_t const SamplingRate = 48'000;

struct Config {
  I2SConfig i2s;
  AdapterSpiConfig adapter;
  uint power_led;
};

Config const config{

    .i2s = {.data = 12, .clock_ws_base = 10, .pio = pio0},

    .adapter{
        .mosi = 16, .miso = 19, .clock = 18, .chip_select = 17, .spi = spi0},

    .power_led = PICO_DEFAULT_LED_PIN

};

PicoRandom random{};
I2S i2s{config.i2s, SamplingRate};

Synth synth{SamplingRate, random};

AdapterSpi adapter_spi{config.adapter};
queue_t messages{};

namespace core_0 {

void task() {
  adapter_spi.init();

  for (;;) {
    auto const message = receive_message(adapter_spi);
    queue_add_blocking(&messages, &message);
  }
}

} // namespace core_0

namespace core_1 {

void task() {
  for (;;) {
    auto const sample = synth.next_sample();
    i2s.output_sample(sample.left, sample.right);
    Message message{};
    if (queue_try_remove(&messages, &message)) {
      synth.handle(message);
    }
  }
}

} // namespace core_1

} // namespace

int main() {
  bi_decl(bi_3pins_with_names(config.i2s.clock_ws_base, "BCLK",
                              config.i2s.clock_ws_base + 1, "WSEL",
                              config.i2s.data, "DIN"));
  bi_decl(bi_2pins_with_func(config.adapter.mosi, config.adapter.chip_select,
                             GPIO_FUNC_SPI));
  bi_decl(bi_2pins_with_func(config.adapter.clock, config.adapter.miso,
                             GPIO_FUNC_SPI));
  bi_decl(bi_1pin_with_name(config.power_led, "LED"));

  gpio_init(config.power_led);
  gpio_set_dir(config.power_led, true);
  gpio_put(config.power_led, false);

  queue_init(&messages, sizeof(Message), 4);

  multicore_launch_core1(core_1::task);

  core_0::task();
}
