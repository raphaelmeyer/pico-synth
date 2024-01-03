#include "push_button.h"

#include "gpio_interrupt_handler.h"

#include <hardware/gpio.h>
#include <hardware/timer.h>

namespace {

constexpr uint64_t const push_threshold = 50'000ull;

} // namespace

void PushButton::init(GpioInterruptHandler &irq) {
  gpio_init(config_.gpio);
  gpio_set_dir(config_.gpio, GPIO_IN);
  gpio_pull_down(config_.gpio);

  irq.subscribe(config_.gpio, [this] { on_event(); });
}

bool PushButton::pop_pushed() {
  auto const result = pushed_;
  pushed_ = false;
  return result;
}

void PushButton::on_event() {
  auto const current = gpio_get(config_.gpio);
  if (current && not previous_) {
    push_time_ = time_us_64();
  } else if (not current && previous_) {
    auto const now = time_us_64();
    if (now - push_time_ > push_threshold) {
      pushed_ = true;
    }
  }
  previous_ = current;
}
