#pragma once

#include <array>
#include <cstdint>

using Word = std::array<uint8_t, 2>;

class Transceiver {
public:
  virtual void send(Word data) = 0;
  virtual Word receive() = 0;
};
