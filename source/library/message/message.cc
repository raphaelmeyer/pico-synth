#include "message.h"

namespace {
namespace cmd {
constexpr const uint8_t trigger = 0x01;
constexpr const uint8_t release = 0x02;
constexpr const uint8_t write_register = 0x10;
} // namespace cmd
} // namespace

std::array<uint8_t, 2> Message::encode() const {
  std::array<uint8_t, 2> data{};
  data[0] = address;

  if (std::holds_alternative<Trigger>(command)) {
    data[1] = cmd::trigger;

  } else if (std::holds_alternative<Release>(command)) {
    data[1] = cmd::release;

  } else if (auto *const write_reg = std::get_if<WriteRegister>(&command);
             write_reg != nullptr) {
    data[1] = cmd::write_register | static_cast<uint8_t>(write_reg->reg);
  }

  return data;
}

Message Message::decode(std::array<uint8_t, 2> data) {
  if (data[1] == cmd::trigger) {
    return {.address = data[0], .command = Trigger{}};

  } else if (data[1] == cmd::release) {
    return {.address = data[0], .command = Release{}};

  } else if ((data[1] & 0xF0) == cmd::write_register) {
    return {.address = data[0],
            .command =
                WriteRegister{.reg = static_cast<Register>(data[1] & 0x0F)}};
  }

  return {};
}

Message decode_message(std::array<uint8_t, 4> data) {
  if (data[1] == cmd::trigger) {
    return {.address = data[0], .command = Trigger{}};

  } else if (data[1] == cmd::release) {
    return {.address = data[0], .command = Release{}};

  } else if ((data[1] & 0xF0) == cmd::write_register) {
    return {.address = data[0],
            .command = WriteRegister{
                .reg = static_cast<Register>(data[1] & 0x0F),
                .data = static_cast<uint16_t>(data[2] | (data[3] << 8))}};
  }

  return {};
}

bool message_has_data(std::array<uint8_t, 4> data) { return data[1] > 0x0F; }
