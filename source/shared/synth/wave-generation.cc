#include "wave-generation.h"

#include "tick.h"

#include "synth/device/random.h"

uint16_t None::value(Tick) { return 0; }

uint16_t Noise::value(Tick tick) {
  if (tick.current == 0) {
    noise1_ = random_.value() % 32768;
  } else if (tick.current % (tick.ticks_per_period / 2) == 0) {
    noise2_ = random_.value() % 16384;
  } else if (tick.current % (tick.ticks_per_period / 4) == 0) {
    noise3_ = random_.value() % 8192;
  } else if (tick.current % (tick.ticks_per_period / 8) == 0) {
    noise4_ = random_.value() % 4096;
  }
  return noise1_ + noise2_ + noise3_ + noise4_;
}

uint16_t Sawtooth::value(Tick tick) {
  return (65535 * tick.current) / tick.ticks_per_period;
}

uint16_t Square::value(Tick tick) {
  return tick.current < (tick.ticks_per_period / 2) ? 65535 : 0;
}

uint16_t Triangle::value(Tick tick) {
  auto const half = tick.ticks_per_period / 2;
  if (tick.current <= half) {
    return 65535 * tick.current / half;
  }
  return 65535 * (tick.ticks_per_period - tick.current) / half;
}
