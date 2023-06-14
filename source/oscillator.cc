#include "oscillator.h"

namespace {
constexpr uint16_t offset(uint16_t frequency, uint sampling_frequency) {
  return 65535ul * frequency / sampling_frequency;
}

} // namespace

Oscillator::Oscillator(Config::Audio const &config_, uint16_t frequency)
    : frequency_{frequency}, offset_{offset(frequency,
                                            config_.sampling_frequency)} {}

uint16_t Oscillator::next_value() {
  value_ = (value_ + offset_) % 65536;
  return value_;
}

void Oscillator::set_type([[maybe_unused]] Form form) {}
void Oscillator::set_frequency([[maybe_unused]] uint16_t frequency) {}
