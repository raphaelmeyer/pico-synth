#pragma once

#include <array>
#include <cstdint>
#include <variant>

enum class Register : uint8_t { Volume = 0x6 };

struct Trigger {};
struct Release {};
struct SetRegister {
  Register reg{};
  uint16_t value{};
};

using Command = std::variant<Trigger, Release, SetRegister>;

struct Message {
  uint8_t address{};
  Command command{};
};

struct Encoded {
  std::array<uint8_t, 2> header{};
};

Encoded encode(Message const &message);
Message decode(Encoded const &message);
