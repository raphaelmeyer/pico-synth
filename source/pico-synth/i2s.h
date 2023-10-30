#pragma once

#include "i2s_config.h"

#include <cstdint>
#include <hardware/pio.h>

class I2S {
public:
  I2S(config::I2S const &config, uint32_t sampling_rate)
      : config_{config}, sampling_rate_{sampling_rate} {}

  void init();

  void output_sample(uint16_t left, uint16_t right);

private:
  int sm_{};

  config::I2S const config_;
  uint32_t const sampling_rate_;
};
