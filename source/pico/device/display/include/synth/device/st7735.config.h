#pragma once

#include <hardware/spi.h>

struct St7735Config {
  uint const clock;
  uint const mosi;
  uint const chip_select;
  uint const data_command;
  uint const reset;

  spi_inst_t *const spi;
};
