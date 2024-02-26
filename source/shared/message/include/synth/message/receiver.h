#pragma once

#include "types.h"

class Receiver {
public:
  virtual Word receive() = 0;
};
