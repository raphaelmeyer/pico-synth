#include "message.h"
#include <cstdint>

namespace {
namespace cmd {
constexpr const uint8_t trigger = 0x01;
constexpr const uint8_t release = 0x01;
constexpr const uint8_t write_register = 0x10;
} // namespace cmd
} // namespace

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
