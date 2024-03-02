#pragma once

#include "receiver.h"
#include "sender.h"

#include <synth/wave-form.h>

#include <functional>
#include <variant>

struct Trigger {};

struct Release {};

struct SetFrequency {
  uint16_t frequency;
};

struct SetVolume {
  uint16_t volume;
};

struct SetAttack {
  uint16_t attack;
};

struct SetDecay {
  uint16_t decay;
};

struct SetSustain {
  uint16_t sustain;
};

struct SetRelease {
  uint16_t release;
};

struct SetWaveForm {
  WaveForm wave{};
};

using Command =
    std::variant<std::monostate, Trigger, Release, SetFrequency, SetVolume,
                 SetAttack, SetDecay, SetSustain, SetRelease, SetWaveForm>;

struct Message {
  uint8_t address{};
  Command command{};
};

void send_message(Message const &message, Sender &sender);

Message receive_message(Receiver &receiver);
