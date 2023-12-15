#include "message.h"

namespace {
namespace cmd {
constexpr const uint8_t trigger = 0x01;
constexpr const uint8_t release = 0x02;
constexpr const uint8_t write_register = 0x10;
} // namespace cmd
} // namespace

Word Message::encode() const {
  Word header{address, 0};

  if (std::holds_alternative<Trigger>(command)) {
    header[1] = cmd::trigger;

  } else if (std::holds_alternative<Release>(command)) {
    header[1] = cmd::release;

  } else if (auto *const write_reg = std::get_if<WriteRegister>(&command);
             write_reg != nullptr) {
    header[1] = cmd::write_register | static_cast<uint8_t>(write_reg->reg);
  }

  return header;
}

std::optional<Word> Message::data() const {
  if (auto *const write_reg = std::get_if<WriteRegister>(&command);
      write_reg != nullptr) {
    return Word{static_cast<uint8_t>(write_reg->data & 0xFF),
                static_cast<uint8_t>(write_reg->data >> 8)};
  }

  return {};
}

bool Message::has_data(Word header) { return (header[1] > 0x0F); }

Message Message::decode(Word header, Word data) {
  if (header[1] == cmd::trigger) {
    return {.address = header[0], .command = Trigger{}};

  } else if (header[1] == cmd::release) {
    return {.address = header[0], .command = Release{}};

  } else if ((header[1] & 0xF0) == cmd::write_register) {
    return {.address = header[0],
            .command = WriteRegister{
                .reg = static_cast<Register>(header[1] & 0x0F),
                .data = static_cast<uint16_t>(data[0] | (data[1] << 8))}};
  }

  return {};
}
