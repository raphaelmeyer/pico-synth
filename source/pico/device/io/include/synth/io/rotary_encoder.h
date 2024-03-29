#pragma once

#include "rotary_encoder.config.h"

#include <functional>

class GpioIrq;

enum class Move { Left, Right };

class RotaryEncoder {
public:
  RotaryEncoder(RotaryEncoderConfig const &config,
                std::function<void(int)> on_rotate);

  void init(GpioIrq &irq);
  void task();

private:
  void on_irq();

  enum class State { Idle, CW, CCW };

  State state_{State::Idle};
  bool previous_a_{};

  int steps_{};
  uint64_t last_{};

  RotaryEncoderConfig const config_;
  std::function<void(int)> on_rotate_;
};
