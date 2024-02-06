#include "message.h"

namespace {

namespace cmd {
constexpr const uint8_t trigger = 0x01;
constexpr const uint8_t release = 0x02;
constexpr const uint8_t set_register = 0x10;
} // namespace cmd

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

} // namespace

Encoded encode(Message const &message) {
  return std::visit(
      overloaded{[&](Trigger const &) {
                   return Encoded{.header = {message.address, cmd::trigger}};
                 },
                 [&](Release const &) {
                   return Encoded{.header = {message.address, cmd::release}};
                 },
                 [&](SetRegister const &set) {
                   return Encoded{
                       .header = {message.address,
                                  static_cast<uint8_t>(
                                      cmd::set_register |
                                      static_cast<uint8_t>(set.reg))}};
                 }},
      message.command);
}

Message decode(Encoded const &message) {
  auto const decode_command = [&]() -> Command {
    auto const command = message.header[1];

    if (command == cmd::trigger) {
      return Trigger{};
    }

    if (command == cmd::release) {
      return Release{};
    }

    if ((command & 0xf0) == cmd::set_register) {
      return SetRegister{.reg = static_cast<Register>(command & 0xf),
                         .value = 0x0000};
    }

    return {};
  };

  return {.address = message.header[0], .command = decode_command()};
}
