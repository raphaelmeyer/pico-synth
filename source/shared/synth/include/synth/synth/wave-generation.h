#pragma once

#include "tick.h"

#include <concepts>
#include <cstdint>

template <typename T>
concept WaveGeneration = requires(T gen) {
  { gen.value(Tick{}) } -> std::same_as<uint16_t>;
};

class None {
public:
  uint16_t value(Tick tick);
};

class Sawtooth {
public:
  uint16_t value(Tick tick);
};

class Triangle {
public:
  uint16_t value(Tick tick);
};
