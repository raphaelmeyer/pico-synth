#pragma once

#include "channel_config.h"
#include "control_config.h"

#include <array>

struct Message;

class Control {
public:
  Control(config::Control const &config, Channels &channels);

  Message read();

  void dispatch(Message const &message);

private:
  config::Control const config_;
  Channels &channels_;
};
