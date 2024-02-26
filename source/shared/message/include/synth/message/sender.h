#pragma once

#include "types.h"

class Sender {
public:
  virtual void send(Word data) = 0;
};
