#pragma once

#include "push_button_config.h"

#include <display_config.h>
#include <rotary_encoder_config.h>
#include <synth_config.h>

struct Config {
  config::Display display;
  config::Synth synth;
  config::RotaryEncoder select;
  config::PushButton confirm;

  int power_led;
};
