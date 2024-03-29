#pragma once

#include "push_button.config.h"

#include <functional>

class GpioIrq;

class PushButton {
public:
  PushButton(PushButtonConfig const &config, std::function<void()> on_click);

  void init(GpioIrq &irq);

private:
  void on_irq();

  bool previous_{};
  uint64_t push_time_{};

  PushButtonConfig const config_;
  std::function<void()> on_click_;
};
