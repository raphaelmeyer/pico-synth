#pragma once

#include <envelope-generator.h>
#include <oscillator.h>

#include "tone-generator.h"

struct Channel {
  Channel(config::Synth config, Random &random)
      : oscillator{config, random}, envelope_generator{config, oscillator},
        tone_generator{oscillator, envelope_generator} {}

  Oscillator oscillator;
  EnvelopeGenerator envelope_generator;
  ToneGenerator tone_generator;
};
