#pragma once

#include "tick.h"

#include <cstdint>

class TickCounter {
public:
  TickCounter(uint32_t sampling_rate);
  Tick next();

  void set_frequency(uint16_t frequency);

private:
  uint32_t const sampling_rate_;

  int current_tick_{};
  int ticks_per_period_{};

  int missing_ticks_per_second_{};
  int correction_accumulator_{};
  int correction_tick_{};
  bool stretch_next_period_{};
};
