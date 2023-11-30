#pragma once

#include "control_config.h"

#include <array>

struct Message;

class Control {
public:
  Control(config::Control const &config);

  Message read();

private:
  config::Control const config_;
};
