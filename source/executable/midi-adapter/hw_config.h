#pragma once

#include "synth_config.h"

#include <display_config.h>

struct Config {
  config::Display display;
  config::Synth synth;

  uint led;
};
