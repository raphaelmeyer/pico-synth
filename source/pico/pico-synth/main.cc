#include "adapter_spi.config.h"
#include "adapter_spi.h"

#include <pico/binary_info.h>
#include <pico/multicore.h>

#include <hardware/gpio.h>

namespace {

struct Config {
  AdapterSpiConfig adapter;
};

Config const config{.adapter{
    .mosi = 16, .miso = 19, .clock = 18, .chip_select = 17, .spi = spi0}};

AdapterSpi adapter_spi{config.adapter};

namespace core_0 {

void task() {
  for (;;) {
  }
}

} // namespace core_0

namespace core_1 {

void task() {
  adapter_spi.init();

  for (;;) {
  }
}

} // namespace core_1

} // namespace

int main() {
  bi_decl(bi_2pins_with_func(config.adapter.mosi, config.adapter.chip_select,
                             GPIO_FUNC_SPI));
  bi_decl(bi_2pins_with_func(config.adapter.clock, config.adapter.miso,
                             GPIO_FUNC_SPI));
  bi_decl(bi_1pin_with_name(PICO_DEFAULT_LED_PIN, "LED"));

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, true);
  gpio_put(PICO_DEFAULT_LED_PIN, false);

  multicore_launch_core1(core_1::task);

  core_0::task();
}
