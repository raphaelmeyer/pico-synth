#pragma once

#include <pico/types.h>

namespace config {

struct RotaryEncoder {
  uint const gpio_a;
  uint const gpio_b;
};

} // namespace config
