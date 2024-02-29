#pragma once

#include "source.h"
#include "tick-counter.h"
#include "wave-generation.h"

#include <synth/wave-form.h>

#include <variant>

class Random;

class Oscillator : public Source {
public:
  Oscillator(uint32_t sampling_rate, Random &random);

  uint16_t next_value() override;

  void set_type(WaveForm form);
  void set_frequency(uint16_t frequency);

private:
  Random &random_;

  TickCounter counter_;
  std::variant<None, Noise, Sawtooth, Square, Triangle> wave_generation_{};
};
