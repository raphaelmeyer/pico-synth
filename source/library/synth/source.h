#pragma once

#include <pico/types.h>

class Source {
public:
  virtual uint16_t next_value() = 0;
};
