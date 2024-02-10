#include "wave-generation.h"

#include "tick.h"

uint16_t None::value(Tick) { return 0; }

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
