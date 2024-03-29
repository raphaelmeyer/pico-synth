#include "push_button.h"

#include "gpio_irq.h"

#include <hardware/gpio.h>
#include <hardware/timer.h>

namespace {

constexpr uint64_t const push_threshold = 50'000ull;

} // namespace

PushButton::PushButton(PushButtonConfig const &config,
                       std::function<void()> on_click)
    : config_{config}, on_click_{on_click} {}

void PushButton::init(GpioIrq &irq) {
  gpio_init(config_.gpio);
  gpio_set_dir(config_.gpio, GPIO_IN);
  gpio_pull_down(config_.gpio);

  irq.subscribe(config_.gpio, [this] { on_irq(); });
}

void PushButton::on_irq() {
  auto const current = gpio_get(config_.gpio);
  if (current && not previous_) {
    push_time_ = time_us_64();
  } else if (not current && previous_) {
    auto const now = time_us_64();
    if (now - push_time_ > push_threshold) {
      on_click_();
    }
  }
  previous_ = current;
}
