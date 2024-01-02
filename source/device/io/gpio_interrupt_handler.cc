#include "gpio_interrupt_handler.h"

#include <hardware/gpio.h>

#include <set>

queue_t GpioInterruptHandler::events_{};

void GpioInterruptHandler::init() {
  queue_init(&events_, sizeof(uint), 16);

  gpio_set_irq_callback(irq_handler);
  irq_set_enabled(IO_IRQ_BANK0, true);
}

void GpioInterruptHandler::task() {
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

void GpioInterruptHandler::subscribe(uint gpio, std::function<void()> handler) {
  handlers_[gpio] = handler;
  gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}

void GpioInterruptHandler::irq_handler(uint gpio, uint32_t) {
  queue_try_add(&events_, &gpio);
}
