#pragma once

#include <hardware/spi.h>

namespace config {

struct Control {
  uint const clock;
  uint const mosi;
  uint const chip_select;

  spi_inst_t *const spi;
};

} // namespace config
