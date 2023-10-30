#pragma once

#include "i2s_config.h"

struct Config {
  struct Audio {
    uint32_t const sampling_frequency;
  };

  Audio const audio{};
  config::I2S const i2s{};
};
