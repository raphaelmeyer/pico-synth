#include "rotary_encoder.h"

#include "gpio_interrupt_handler.h"

#include <hardware/gpio.h>

void RotaryEncoder::init(GpioInterruptHandler &irq) {
  gpio_init(config_.gpio_a);
  gpio_set_dir(config_.gpio_a, GPIO_IN);
  gpio_pull_up(config_.gpio_a);

  gpio_init(config_.gpio_b);
  gpio_set_dir(config_.gpio_b, GPIO_IN);
  gpio_pull_up(config_.gpio_b);

  irq.subscribe(config_.gpio_a, [this] { on_event(); });
}

int RotaryEncoder::steps() {
  auto const result = steps_;
  steps_ = 0;
  return result;
}

void RotaryEncoder::on_event() {
  auto const current_a = gpio_get(config_.gpio_a);
  if (current_a != previous_a_) {
    auto const current_b = gpio_get(config_.gpio_b);

    switch (state_) {
    case State::Idle: {
      if (not current_a) {
        if (current_b) {
          state_ = State::CW;
        } else {
          state_ = State::CCW;
        }
      }
    } break;

    case State::CW: {
      if (current_a && (not current_b)) {
        ++steps_;
      }
      state_ = State::Idle;
    } break;

    case State::CCW: {
      if (current_a && current_b) {
        --steps_;
      }
      state_ = State::Idle;
    } break;
    }

    previous_a_ = current_a;
  }
}
