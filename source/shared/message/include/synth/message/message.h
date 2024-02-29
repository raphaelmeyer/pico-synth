#pragma once

#include "receiver.h"
#include "sender.h"

#include <synth/wave-form.h>

#include <functional>
#include <variant>

enum class Register : uint8_t {
  Reserved = 0x0,
  Frequency = 0x1,
  Attack = 0x2,
  Decay = 0x3,
  Sustain = 0x4,
  Release = 0x5,
  Volume = 0x6,
  WaveForm = 0x7
};

struct Trigger {};
struct Release {};
struct SetRegister {
  Register reg{};
  uint16_t value{};
};
struct SetWaveForm {
  WaveForm wave{};
};

using Command = std::variant<Trigger, Release, SetRegister, SetWaveForm>;

struct Message {
  uint8_t address{};
  Command command{};
};

void send_message(Message const &message, Sender &sender);

Message receive_message(Receiver &receiver);
