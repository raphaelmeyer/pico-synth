#pragma once

#include <hardware/structs/pio.h>

struct Config {
  struct Audio {
    uint32_t const sampling_frequency;
  };

  struct I2S {
    uint const data;
    uint const clock_ws_base;

    pio_hw_t *const pio;
  };

  Audio const audio{};
  I2S const i2s{};
};
