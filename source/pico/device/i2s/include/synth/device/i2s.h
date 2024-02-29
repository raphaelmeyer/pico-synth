#pragma once

#include "i2s.config.h"

#include <cstdint>
#include <hardware/pio.h>

class I2S {
public:
  I2S(I2SConfig const &config, uint32_t sampling_rate);

  void init();

  void output_sample(uint16_t left, uint16_t right);

private:
  int sm_{};

  I2SConfig const config_;
  uint32_t const sampling_rate_;
};
