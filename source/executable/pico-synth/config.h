#pragma once

#include "i2s_config.h"
#include "synth_config.h"

struct Config {

  config::Synth const synth{};
  config::I2S const i2s{};
};
