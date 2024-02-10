#include "wave-generation.h"

#include "tick.h"

uint16_t None::value(Tick) { return 0; }

uint16_t Sawtooth::value(Tick tick) {
  return (65535 * tick.current) / tick.ticks_per_period;
}
