#pragma once

#include <cstdint>

class Random {
public:
  virtual uint32_t value() = 0;
};
