#pragma once

#include "tick.h"

#include <concepts>
#include <cstdint>

template <typename T>
concept WaveGeneration = requires(T gen) {
  { gen.value(Tick{}) } -> std::same_as<uint16_t>;
};

class Random;

class None {
public:
  uint16_t value(Tick tick);
};

class Noise {
public:
  Noise(Random &random) : random_{random} {}

  uint16_t value(Tick tick);

private:
  Random &random_;

  uint16_t noise1_{};
  uint16_t noise2_{};
  uint16_t noise3_{};
  uint16_t noise4_{};
};

class Sawtooth {
public:
  uint16_t value(Tick tick);
};

class Square {
public:
  uint16_t value(Tick tick);
};

class Triangle {
public:
  uint16_t value(Tick tick);
};
