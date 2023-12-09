#pragma once

#include <cstdint>

class Source {
public:
  virtual uint16_t next_value() = 0;
};
