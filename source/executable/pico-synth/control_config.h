#pragma once

#include <hardware/spi.h>

namespace config {

struct Control {
  uint const mosi;
  uint const miso;
  uint const clock;
  uint const chip_select;

  spi_inst_t *const spi;
};

} // namespace config
