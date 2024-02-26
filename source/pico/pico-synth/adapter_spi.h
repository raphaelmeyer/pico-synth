#pragma once

#include "adapter_spi.config.h"

#include <synth/message/receiver.h>

class AdapterSpi : public Receiver {
public:
  AdapterSpi(AdapterSpiConfig const config);

  void init();

  Word receive() override;

private:
  AdapterSpiConfig const config_;
};
