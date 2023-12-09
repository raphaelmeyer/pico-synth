#pragma once

#include <random.h>

class PicoRandom : public Random {
public:
  uint32_t value() override;
};
