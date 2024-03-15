#pragma once

#include "synth/definitions.h"
#include "synth/wave-form.h"

#include <array>
#include <cstdint>

enum class Property { Volume, Attack, Decay, Sustain, Release, WaveForm };

struct Channel {
  uint16_t volume;
  uint16_t attack;
  uint16_t decay;
  uint16_t sustain;
  uint16_t release;

  WaveForm wave;
};

struct Model {
  std::array<Channel, NumOscillators> channels{};
};
