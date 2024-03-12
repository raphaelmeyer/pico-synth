#pragma once

#include <array>
#include <cstdint>

using MidiPacket = std::array<uint8_t, 4>;

class Sender;

class MidiControl {
public:
  MidiControl(Sender &sender);

  void handle(MidiPacket const &packet);

private:
  void play(uint8_t channel, uint8_t note);
  void release(uint8_t channel);

  Sender &sender_;
};
