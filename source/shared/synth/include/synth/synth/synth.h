#pragma once

#include "envelope-generator.h"
#include "oscillator.h"

#include <synth/definitions.h>
#include <synth/message/message.h>

struct Sample {
  uint16_t left;
  uint16_t right;
};

class Synth {
public:
  Synth(uint32_t sampling_rate, Random &random);

  void handle(Message const &message);
  Sample next_sample();

private:
  struct Voice {
    Voice(uint32_t sampling_rate, Random &random)
        : oscillator{sampling_rate, random},
          envelope_generator{sampling_rate, oscillator} {}

    Oscillator oscillator;
    EnvelopeGenerator envelope_generator;
  };

  std::array<Voice, NumOscillators> voices_;
};
