#pragma once

#include "config.h"

#include <pico/types.h>

enum class Form { Square, Sawtooth, Sine };

class Oscillator {
public:
  Oscillator(Config::Audio const &config_, uint16_t frequency);

  uint16_t next_value();

  void set_type([[maybe_unused]] Form form);
  void set_frequency([[maybe_unused]] uint16_t frequency);

private:
  uint16_t value_ = 0;

  uint16_t frequency_{};
  uint16_t offset_{};
};
