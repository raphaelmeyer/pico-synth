#include "tick-counter.h"

TickCounter::TickCounter(uint32_t sampling_rate)
    : sampling_rate_{sampling_rate} {}

Tick TickCounter::next() {
  ++current_tick_;
  if (current_tick_ >= (ticks_per_period_ + correction_tick_)) {
    current_tick_ = 0;
    if (stretch_next_period_) {
      stretch_next_period_ = false;
      correction_tick_ = 1;
    } else {
      correction_tick_ = 0;
    }
  }

  correction_accumulator_ += missing_ticks_per_second_;
  if (correction_accumulator_ >= static_cast<int>(sampling_rate_)) {
    correction_accumulator_ -= sampling_rate_;
    stretch_next_period_ = true;
  }

  return {current_tick_, ticks_per_period_ + correction_tick_};
}

void TickCounter::set_frequency(uint16_t frequency) {
  ticks_per_period_ = (4 * sampling_rate_) / frequency;
  current_tick_ = 0;

  missing_ticks_per_second_ =
      sampling_rate_ - ((ticks_per_period_ * frequency) / 4);

  correction_accumulator_ = 0;
  correction_tick_ = 0;
  stretch_next_period_ = false;
}
