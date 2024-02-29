#pragma once

#include <hardware/structs/pio.h>

struct I2SConfig {
  uint const data;
  uint const clock_ws_base;

  pio_hw_t *const pio;
};
