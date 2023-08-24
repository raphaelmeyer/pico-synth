#include "oscillator.h"

Oscillator::Oscillator(Config::Audio const &config) : config_{config} {}

uint16_t Oscillator::next_value() {
  if (ticks_ > period_) {
    ticks_ = 0;
  }

  // saw tooth
  //  uint16_t value = 16384 * ticks_ / period_;

  // square
  uint16_t value = ticks_ < (period_ / 2) ? 16384 : 0;

  ++ticks_;

  return value;
}

void Oscillator::set_type(Form form) { form_ = form; }

void Oscillator::set_frequency(uint16_t frequency) {
  frequency_ = frequency;
  period_ = config_.sampling_frequency / frequency;
}
