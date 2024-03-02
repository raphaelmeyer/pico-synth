#include "message.h"
#include <variant>

namespace {

namespace cmd {
constexpr uint8_t const trigger = 0x01;
constexpr uint8_t const release = 0x02;
constexpr uint8_t const set_register = 0x10;
} // namespace cmd

namespace reg {
constexpr uint8_t const freqeuncy = 0x1;
constexpr uint8_t const attack = 0x2;
constexpr uint8_t const decay = 0x3;
constexpr uint8_t const sustain = 0x4;
constexpr uint8_t const release = 0x5;
constexpr uint8_t const volume = 0x6;
constexpr uint8_t const wave = 0x7;
} // namespace reg

namespace wave {
constexpr uint8_t const noise = 0x0;
constexpr uint8_t const square = 0x1;
constexpr uint8_t const triangle = 0x2;
constexpr uint8_t const sawtooth = 0x3;
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

Word encode_data(uint16_t data) {
  return {static_cast<uint8_t>(data & 0xff),
          static_cast<uint8_t>((data >> 8) & 0xff)};
}

uint16_t decode_data(Word data) {
  return static_cast<uint16_t>(data[0] | (data[1] << 8));
}

} // namespace

void send_message(Message const &message, Sender &sender) {
  auto const address = message.address;

  std::visit(overloaded{

                 [address, &sender](Trigger) {
                   sender.send({address, cmd::trigger});
                 },

                 [address, &sender](Release) {
                   sender.send({address, cmd::release});
                 },

                 [address, &sender](SetFrequency set) {
                   sender.send({address, cmd::set_register | reg::freqeuncy});
                   sender.send(encode_data(set.frequency));
                 },

                 [address, &sender](SetVolume set) {
                   sender.send({address, cmd::set_register | reg::volume});
                   sender.send(encode_data(set.volume));
                 },

                 [address, &sender](SetAttack set) {
                   sender.send({address, cmd::set_register | reg::attack});
                   sender.send(encode_data(set.attack));
                 },

                 [address, &sender](SetDecay set) {
                   sender.send({address, cmd::set_register | reg::decay});
                   sender.send(encode_data(set.decay));
                 },

                 [address, &sender](SetSustain set) {
                   sender.send({address, cmd::set_register | reg::sustain});
                   sender.send(encode_data(set.sustain));
                 },

                 [address, &sender](SetRelease set) {
                   sender.send({address, cmd::set_register | reg::release});
                   sender.send(encode_data(set.release));
                 },

                 [address, &sender](SetWaveForm set) {
                   sender.send({address, cmd::set_register | reg::wave});
                   sender.send({encode_wave_form(set.wave), 0});
                 },

                 [](auto) {}

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

      switch (command & 0xf) {
      case reg::freqeuncy:
        return SetFrequency{decode_data(data)};

      case reg::volume:
        return SetVolume{decode_data(data)};

      case reg::attack:
        return SetAttack{decode_data(data)};

      case reg::decay:
        return SetDecay{decode_data(data)};

      case reg::sustain:
        return SetSustain{decode_data(data)};

      case reg::release:
        return SetRelease{decode_data(data)};

      case reg::wave:
        return SetWaveForm{decode_wave_form(data[0] & 0xf)};

      default:
        return {};
      }
    }

    return {};
  };

  return {.address = header[0], .command = decode_command()};
}
