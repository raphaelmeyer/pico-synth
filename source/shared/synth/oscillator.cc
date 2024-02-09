#include "oscillator.h"

Oscillator::Oscillator(uint32_t sampling_rate)
    : sampling_rate_{sampling_rate} {}

uint16_t Oscillator::next_value() {
  uint16_t value =
      (65535 * tick_in_period) / (ticks_per_period + correction_tick);

  ++tick_in_period;
  if (tick_in_period >= (ticks_per_period + correction_tick)) {
    tick_in_period = 0;
    if (stretch_next_period) {
      stretch_next_period = false;
      correction_tick = 1;
    } else {
      correction_tick = 0;
    }
  }

  correction_accumulator += missing_ticks_per_second;
  if (correction_accumulator >= sampling_rate_) {
    correction_accumulator -= sampling_rate_;
    stretch_next_period = true;
  }

  return value;
}

void Oscillator::set_type(WaveForm) {}

void Oscillator::set_frequency(uint16_t frequency) {
  ticks_per_period = (4 * sampling_rate_) / frequency;

  missing_ticks_per_second =
      sampling_rate_ - ((ticks_per_period)*frequency / 4);

  correction_accumulator = 0;
  correction_tick = 0;
  tick_in_period = 0;
  stretch_next_period = false;
}
