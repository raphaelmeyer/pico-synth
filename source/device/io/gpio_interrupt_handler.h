#pragma once

#include <pico/util/queue.h>

#include <functional>
#include <map>

class GpioInterruptHandler {
public:
  void init();
  void task();

  void subscribe(uint gpio, std::function<void()> handler);

private:
  static void irq_handler(uint gpio, uint32_t);

  static queue_t events_;

  std::map<uint, std::function<void()>> handlers_{};
};
