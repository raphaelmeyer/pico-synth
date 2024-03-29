#pragma once

#include "rotary_encoder_config.h"

class GpioInterruptHandler;

class RotaryEncoder {
public:
  RotaryEncoder(config::RotaryEncoder const &config) : config_{config} {}

  void init(GpioInterruptHandler &irq);

  int steps();

private:
  void on_event();

  enum class State { Idle, CW, CCW };

  config::RotaryEncoder const config_;

  State state_{State::Idle};
  bool previous_a_{};
  int steps_{};
};
