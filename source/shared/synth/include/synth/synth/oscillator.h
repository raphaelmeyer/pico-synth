#pragma once

#include "source.h"
#include "wave-form.h"

class Oscillator : public Source {
public:
  Oscillator(uint32_t sampling_rate);

  uint16_t next_value() override;

  void set_type(WaveForm form);
  void set_frequency(uint16_t frequency);

private:
  uint32_t const sampling_rate_;

  int ticks_per_period{};
  int missing_ticks_per_second{};
  int correction_accumulator{};
  int correction_tick{};
  int tick_in_period{};
  bool stretch_next_period{};
};
