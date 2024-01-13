#pragma once

#include "pico/util/queue.h"

#include <variant>

class RotaryEncoder;
class PushButton;

class Knob {
public:
  struct Push {};
  struct Rotate {
    int steps{};
  };
  using Event = std::variant<std::monostate, Push, Rotate>;

  Knob(RotaryEncoder &select, PushButton &confirm)
      : select_{select}, confirm_{confirm} {}

  void init();
  void task();

  Event event();

private:
  RotaryEncoder &select_;
  PushButton &confirm_;

  queue_t events_{};
  uint64_t last{0};
};
