#include "rotary_encoder.h"

#include "gpio_irq.h"

#include <hardware/gpio.h>

namespace {
constexpr uint64_t const Period = 100'000ull;
}

RotaryEncoder::RotaryEncoder(RotaryEncoderConfig const &config,
                             std::function<void(int)> on_rotate)
    : config_{config}, on_rotate_{on_rotate} {}

void RotaryEncoder::init(GpioIrq &irq) {
  gpio_init(config_.gpio_a);
  gpio_set_dir(config_.gpio_a, GPIO_IN);
  gpio_pull_up(config_.gpio_a);

  gpio_init(config_.gpio_b);
  gpio_set_dir(config_.gpio_b, GPIO_IN);
  gpio_pull_up(config_.gpio_b);

  irq.subscribe(config_.gpio_a, [this] { on_irq(); });
}

void RotaryEncoder::task() {
  auto const now = time_us_64();
  if (now - last_ > Period) {
    last_ = now;
    if (steps_) {
      on_rotate_(steps_);
      steps_ = 0;
    }
  }
}

void RotaryEncoder::on_irq() {
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
