#pragma once

#include "source.h"
#include "synth_config.h"

class EnvelopeGenerator : public Source {
public:
  EnvelopeGenerator(config::Synth const &config, Source &source)
      : config_{config}, source_{source} {}

  uint16_t next_value() override;

  void trigger();
  void release();

  void set_attack(uint16_t attack);
  void set_decay(uint16_t decay);
  void set_sustain(uint16_t sustain);
  void set_release(uint16_t release);

private:
  config::Synth const config_;
  Source &source_;

  enum class State { Idle, Attack, Decay, Sustain, Release };
  State state_{State::Idle};

  int32_t attack_{};
  int32_t decay_{};
  uint16_t sustain_{};
  int32_t release_{};

  int32_t ticks_{};
};
