#pragma once

#include "config.h"

#include <hardware/pio.h>

class I2S {
public:
  I2S(Config::I2S const &config, Config::Audio const &audio)
      : config_{config}, audio_{audio} {};

  void init();

  void output_sample(uint16_t left, uint16_t right);

private:
  int sm_{};

  Config::I2S const config_;
  Config::Audio const audio_;
};
