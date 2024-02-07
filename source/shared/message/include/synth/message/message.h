#pragma once

#include <array>
#include <cstdint>
#include <functional>
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

using Word = std::array<uint8_t, 2>;

void send(Message const &message, std::function<void(Word)> send_word);

Message receive(std::function<Word()> receive_word);
