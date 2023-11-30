#pragma once

#include "control_config.h"
#include "i2s_config.h"
#include "synth_config.h"

struct Config {
  config::Synth const synth{};
  config::I2S const i2s{};

  config::Control const control{};
};
