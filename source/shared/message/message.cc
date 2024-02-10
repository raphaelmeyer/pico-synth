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

void send(Message const &message, std::function<void(Word)> send_word) {
  std::visit(overloaded{

                 [&](Trigger const &) {
                   send_word({message.address, cmd::trigger});
                 },

                 [&](Release const &) {
                   send_word({message.address, cmd::release});
                 },

                 [&](SetRegister const &set) {
                   uint8_t const command = static_cast<uint8_t>(
                       cmd::set_register | static_cast<uint8_t>(set.reg));
                   send_word({message.address, command});
                   send_word({static_cast<uint8_t>(set.value & 0xff),
                              static_cast<uint8_t>((set.value >> 8) & 0xff)});
                 }

             },
             message.command);
}

Message receive(std::function<Word()> receive_word) {
  auto const header = receive_word();

  auto const decode_command = [&]() -> Command {
    auto const command = header[1];

    if (command == cmd::trigger) {
      return Trigger{};
    }

    if (command == cmd::release) {
      return Release{};
    }

    if ((command & 0xf0) == cmd::set_register) {
      auto const data = receive_word();
      return SetRegister{.reg = static_cast<Register>(command & 0xf),
                         .value =
                             static_cast<uint16_t>(data[0] | (data[1] << 8))};
    }

    return {};
  };

  return {.address = header[0], .command = decode_command()};
}