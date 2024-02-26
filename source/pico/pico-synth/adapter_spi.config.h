#pragma once

#include <hardware/spi.h>

struct AdapterSpiConfig {
  uint const mosi;
  uint const miso;
  uint const clock;
  uint const chip_select;

  spi_inst_t *const spi;
};
