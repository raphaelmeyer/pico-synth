#include "control.h"

#include <message.h>

#include <hardware/gpio.h>
#include <hardware/spi.h>

Control::Control(config::Control const &config) : config_{config} {
  spi_init(config_.spi, 25'000'000);
  spi_set_slave(config_.spi, true);

  gpio_set_function(config_.mosi, GPIO_FUNC_SPI);
  gpio_set_function(config.clock, GPIO_FUNC_SPI);
  gpio_set_function(config.chip_select, GPIO_FUNC_SPI);
}

Message Control::read() {
  std::array<uint8_t, 4> buf{};
  spi_read_blocking(config_.spi, 0, buf.data(), 2);
  if (message_has_data(buf)) {
    spi_read_blocking(config_.spi, 0, buf.data() + 2, 2);
  }
  return decode_message(buf);
}
