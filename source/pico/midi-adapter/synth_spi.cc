#include "synth_spi.h"
#include "synth_spi.config.h"

#include <hardware/gpio.h>

SynthSpi::SynthSpi(SynthSpiConfig const config) : config_{config} {}

void SynthSpi::init() {
  spi_init(config_.spi, 5'000'000);

  gpio_set_function(config_.mosi, GPIO_FUNC_SPI);
  gpio_set_function(config_.miso, GPIO_FUNC_SPI);
  gpio_set_function(config_.clock, GPIO_FUNC_SPI);
  gpio_set_function(config_.chip_select, GPIO_FUNC_SPI);
}

void SynthSpi::send(Word data) {
  spi_write_blocking(config_.spi, data.data(), data.size());
}

Word SynthSpi::receive() { return {}; }
