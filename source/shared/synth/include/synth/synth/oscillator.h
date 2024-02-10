#pragma once

#include "source.h"
#include "tick-counter.h"
#include "wave-form.h"
#include "wave-generation.h"

#include <variant>

class Oscillator : public Source {
public:
  Oscillator(uint32_t sampling_rate);

  uint16_t next_value() override;

  void set_type(WaveForm form);
  void set_frequency(uint16_t frequency);

private:
  TickCounter counter_;
  std::variant<None, Sawtooth, Square, Triangle> wave_generation_{};
};
