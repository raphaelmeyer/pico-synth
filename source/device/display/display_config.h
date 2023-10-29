#pragma once

#include <hardware/spi.h>

namespace config {

struct Display {
  uint const clock;
  uint const mosi;
  uint const chip_select;
  uint const data_command;
  uint const reset;

  spi_inst_t *const spi;
};

} // namespace config
