#pragma once

#include "channel_config.h"

class I2S;

class Mixer {
public:
  Mixer(Channels &channels, I2S &i2s);

  void mix();

private:
  Channels &channels_;
  I2S &i2s_;
};
