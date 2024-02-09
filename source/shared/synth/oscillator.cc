#include "oscillator.h"

Oscillator::Oscillator(uint32_t sampling_rate) : counter_{sampling_rate} {}

uint16_t Oscillator::next_value() {
  const auto tick = counter_.next();
  uint16_t value = (65535 * tick.current) / tick.ticks_per_period;

  return value;
}

void Oscillator::set_type(WaveForm) {}

void Oscillator::set_frequency(uint16_t frequency) {
  counter_.set_frequency(frequency);
}
