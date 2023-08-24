#include "oscillator.h"

Oscillator::Oscillator(Config::Audio const &config, uint16_t frequency)
    : frequency_{frequency}, config_{config} {
  offset_ = offset();
}

uint16_t Oscillator::next_value() {
  value_ = (value_ + offset_) % 65536;
  return value_;
}

void Oscillator::set_type([[maybe_unused]] Form form) {}

void Oscillator::set_frequency(uint16_t frequency) {
  frequency_ = frequency;
  value_ = 0;
  offset_ = offset();
}

uint16_t Oscillator::offset() const {
  return 65535ul * frequency_ / config_.sampling_frequency;
}
