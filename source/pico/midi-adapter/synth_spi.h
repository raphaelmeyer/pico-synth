#pragma once

#include "synth_spi.config.h"

#include <synth/message/transceiver.h>

class SynthSpi : public Transceiver {
public:
  SynthSpi(SynthSpiConfig const config);

  void init();

  void send(Word data) override;
  Word receive() override;

private:
  SynthSpiConfig const config_;
};
