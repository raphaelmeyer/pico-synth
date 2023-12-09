#include "oscillator.h"

#include <random.h>

Oscillator::Oscillator(config::Synth const &config, Random &random)
    : config_{config}, random_{random} {}

uint16_t Oscillator::next_value() {
  if (ticks_ > period_) {
    ticks_ = 0;
  }

  uint16_t value = 0;

  if (form_ == Form::Sawtooth) {
    value = 16384 * ticks_ / period_;

  } else if (form_ == Form::Triangle) {
    auto const half = period_ / 2;
    if (ticks_ < half) {
      value = 16384 * ticks_ / half;
    } else {
      value = 16384 * (period_ - ticks_) / half;
    }

  } else if (form_ == Form::Square) {
    value = ticks_ < (period_ / 2) ? 16384 : 0;

  } else if (form_ == Form::Noise) {
    if (ticks_ == 0) {
      noise1_ = random_.value() % 8192;
    } else if (ticks_ % (period_ / 2) == 0) {
      noise2_ = random_.value() % 4096;
    } else if (ticks_ % (period_ / 4) == 0) {
      noise3_ = random_.value() % 2048;
    } else if (ticks_ % (period_ / 8) == 0) {
      noise4_ = random_.value() % 2048;
    }
    value = noise1_ + noise2_ + noise3_ + noise4_;
  }
  ++ticks_;

  return value;
}

void Oscillator::set_type(Form form) { form_ = form; }

void Oscillator::set_frequency(uint16_t frequency) {
  frequency_ = frequency;
  period_ = config_.sampling_rate / frequency;
}
