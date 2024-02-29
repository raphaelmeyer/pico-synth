#include "message.h"

namespace {

namespace cmd {
constexpr const uint8_t trigger = 0x01;
constexpr const uint8_t release = 0x02;
constexpr const uint8_t set_register = 0x10;
} // namespace cmd

namespace wave {
constexpr const uint8_t noise = 0x0;
constexpr const uint8_t square = 0x1;
constexpr const uint8_t triangle = 0x2;
constexpr const uint8_t sawtooth = 0x3;
} // namespace wave

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

uint8_t encode_wave_form(WaveForm wave) {
  switch (wave) {
  default:
  case WaveForm::Noise:
    return wave::noise;

  case WaveForm::Square:
    return wave::square;

  case WaveForm::Triangle:
    return wave::triangle;

  case WaveForm::Sawtooth:
    return wave::sawtooth;
  }
}

WaveForm decode_wave_form(uint8_t wave) {
  switch (wave) {
  default:
  case wave::noise:
    return WaveForm::Noise;

  case wave::square:
    return WaveForm::Square;

  case wave::triangle:
    return WaveForm::Triangle;

  case wave::sawtooth:
    return WaveForm::Sawtooth;
  }
}

} // namespace

void send_message(Message const &message, Sender &sender) {
  std::visit(overloaded{

                 [&](Trigger const &) {
                   sender.send({message.address, cmd::trigger});
                 },

                 [&](Release const &) {
                   sender.send({message.address, cmd::release});
                 },

                 [&](SetRegister const &set) {
                   uint8_t const command = static_cast<uint8_t>(
                       cmd::set_register | static_cast<uint8_t>(set.reg));
                   sender.send({message.address, command});
                   sender.send({static_cast<uint8_t>(set.value & 0xff),
                                static_cast<uint8_t>((set.value >> 8) & 0xff)});
                 },

                 [&](SetWaveForm const &set) {
                   sender.send({message.address,
                                cmd::set_register |
                                    static_cast<uint8_t>(Register::WaveForm)});
                   sender.send({encode_wave_form(set.wave), 0});
                 }

             },
             message.command);
}

Message receive_message(Receiver &receiver) {
  auto const header = receiver.receive();

  auto const decode_command = [&]() -> Command {
    auto const command = header[1];

    if (command == cmd::trigger) {
      return Trigger{};
    }

    if (command == cmd::release) {
      return Release{};
    }

    if ((command & 0xf0) == cmd::set_register) {
      auto const data = receiver.receive();

      if ((command & 0xf) == static_cast<int>(Register::WaveForm)) {
        return SetWaveForm{decode_wave_form(data[0] & 0xf)};
      }

      return SetRegister{.reg = static_cast<Register>(command & 0xf),
                         .value =
                             static_cast<uint16_t>(data[0] | (data[1] << 8))};
    }

    return {};
  };

  return {.address = header[0], .command = decode_command()};
}
