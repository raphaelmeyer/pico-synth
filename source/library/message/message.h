#pragma once

#include <array>
#include <cstdint>
#include <variant>

enum class Register : uint8_t {
  Control = 0x0,
  Frequency = 0x1,
  Attack = 0x2,
  Decay = 0x3,
  Sustain = 0x4,
  Release = 0x5
};

struct Trigger {};
struct Release {};
struct WriteRegister {
  Register reg{};
  uint16_t data{};
};

struct Message {
  uint8_t address{};
  std::variant<Trigger, Release, WriteRegister> command{};
};

Message decode_message(std::array<uint8_t, 4> data);

bool message_has_data(std::array<uint8_t, 4> data);
