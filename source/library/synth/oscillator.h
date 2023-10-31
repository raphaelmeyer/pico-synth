#pragma once

#include "source.h"
#include "synth_config.h"

enum class Form { Sawtooth, Triangle, Square, Noise };

class Oscillator : public Source {
public:
  Oscillator(config::Synth const &config_);

  uint16_t next_value() override;

  void set_type(Form form);
  void set_frequency(uint16_t frequency);

private:
  uint16_t frequency_{440};
  Form form_{};

  int32_t ticks_{};
  int32_t period_{};

  uint16_t noise1_{};
  uint16_t noise2_{};
  uint16_t noise3_{};
  uint16_t noise4_{};

  config::Synth const config_{};
};
