#pragma once

#include "source.h"

class EnvelopeGenerator : public Source {
public:
  EnvelopeGenerator(uint32_t sampling_rate, Source &source);

  uint16_t next_value() override;

  void trigger();
  void release();

  void set_attack(uint16_t attack);
  void set_decay(uint16_t decay);
  void set_sustain(uint16_t sustain);
  void set_release(uint16_t release);
  void set_volume(uint16_t volume);

private:
  uint32_t const sampling_rate_;
  Source &source_;

  enum class State { Idle, Attack, Decay, Sustain, Release };
  State state_{State::Idle};

  int32_t attack_{};
  int32_t decay_{};
  uint16_t sustain_{};
  int32_t release_{};
  int32_t volume_{};

  int32_t ticks_{};
};
