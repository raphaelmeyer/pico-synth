#pragma once

#include <hardware/structs/pio.h>

namespace config {

struct I2S {
  uint const data;
  uint const clock_ws_base;

  pio_hw_t *const pio;
};

} // namespace config
