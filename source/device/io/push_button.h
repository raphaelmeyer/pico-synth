#pragma once

#include "push_button_config.h"

class GpioInterruptHandler;

class PushButton {
public:
  PushButton(config::PushButton const &config) : config_{config} {}

  void init(GpioInterruptHandler &irq);

  bool pop_pushed();

private:
  void on_event();

  config::PushButton const config_;

  bool previous_{};
  uint64_t push_time_{};

  bool pushed_{};
};
