#pragma once

#include "synth/wave-form.h"

#include <cstdint>

struct Model {
  uint16_t volume;
  uint16_t frequency;

  WaveForm wave;
};
