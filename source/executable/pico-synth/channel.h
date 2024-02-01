#pragma once

#include <envelope-generator.h>
#include <oscillator.h>

#include "tone-generator.h"

class Channel {
public:
  Channel(config::Synth const &config, Random &random)
      : tone_generator{oscillator, envelope_generator},
        oscillator{config, random}, envelope_generator{config, oscillator} {}

  ToneGenerator tone_generator;

private:
  Oscillator oscillator;
  EnvelopeGenerator envelope_generator;
};
