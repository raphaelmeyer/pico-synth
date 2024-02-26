#include "adapter_spi.h"
#include "adapter_spi.config.h"

#include <hardware/gpio.h>

AdapterSpi::AdapterSpi(AdapterSpiConfig const config) : config_{config} {}

void AdapterSpi::init() {
  spi_init(config_.spi, 5'000'000);
  spi_set_slave(config_.spi, true);

  gpio_set_function(config_.mosi, GPIO_FUNC_SPI);
  gpio_set_function(config_.miso, GPIO_FUNC_SPI);
  gpio_set_function(config_.clock, GPIO_FUNC_SPI);
  gpio_set_function(config_.chip_select, GPIO_FUNC_SPI);
}

Word AdapterSpi::receive() {
  Word data{};
  spi_read_blocking(config_.spi, 0, data.data(), data.size());
  return data;
}
