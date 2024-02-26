#pragma once

#include "synth_spi.config.h"

#include <synth/message/sender.h>

class SynthSpi : public Sender {
public:
  SynthSpi(SynthSpiConfig const config);

  void init();

  void send(Word data) override;

private:
  SynthSpiConfig const config_;
};
