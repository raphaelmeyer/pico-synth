#include "gpio_irq.h"

#include <hardware/gpio.h>

#include <set>

queue_t GpioIrq::events_{};

void GpioIrq::init() {
  queue_init(&events_, sizeof(uint), 16);

  gpio_set_irq_callback(handler);
  irq_set_enabled(IO_IRQ_BANK0, true);
}

void GpioIrq::task() {
  uint gpio{};
  std::set<uint> events{};
  while (queue_try_remove(&events_, &gpio)) {
    events.insert(gpio);
  }

  for (auto const gpio : events) {
    auto const &handler = handlers_.find(gpio);
    if (handler != handlers_.end()) {
      handler->second();
    }
  }
}

void GpioIrq::subscribe(uint gpio, std::function<void()> handler) {
  handlers_[gpio] = handler;
  gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}

void GpioIrq::handler(uint gpio, uint32_t) { queue_try_add(&events_, &gpio); }
