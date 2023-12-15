#include "control.h"

#include <message.h>

#include <hardware/gpio.h>
#include <hardware/spi.h>

Control::Control(config::Control const &config) : config_{config} {
  spi_init(config_.spi, 25'000'000);
  spi_set_slave(config_.spi, true);

  gpio_set_function(config_.mosi, GPIO_FUNC_SPI);
  gpio_set_function(config_.miso, GPIO_FUNC_SPI);
  gpio_set_function(config_.clock, GPIO_FUNC_SPI);
  gpio_set_function(config_.chip_select, GPIO_FUNC_SPI);
}

Message Control::read() {
  Word header{};
  Word data{};
  spi_read_blocking(config_.spi, 0, header.data(), 2);
  if (Message::has_data(header)) {
    spi_read_blocking(config_.spi, 0, data.data() + 2, 2);
  }
  return Message::decode(header, data);
}
