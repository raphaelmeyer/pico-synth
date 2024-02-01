#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <variant>

enum class Register : uint8_t {
  Control = 0x0,
  Frequency = 0x1,
  Attack = 0x2,
  Decay = 0x3,
  Sustain = 0x4,
  Release = 0x5,
  Volume = 0x6,
};

struct Reserved {};
struct Trigger {};
struct Release {};
struct WriteRegister {
  Register reg{};
  uint16_t data{};
};

using Word = std::array<uint8_t, 2>;

struct Message {
  uint8_t address{};
  std::variant<Reserved, Trigger, Release, WriteRegister> command{};

  Word encode() const;
  std::optional<Word> data() const;

  static bool has_data(Word header);
  static Message decode(Word header, Word data = {});
};
