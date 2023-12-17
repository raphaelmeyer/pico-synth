#pragma once

#include "channel_config.h"
#include "control_config.h"

#include <array>

struct Message;

class Control {
public:
  Control(config::Control const &config, Channels &channels)
      : config_{config}, channels_{channels} {}

  void init();

  Message read();
  void dispatch(Message const &message);

  void setup_demo_instruments();

private:
  config::Control const config_;
  Channels &channels_;
};
