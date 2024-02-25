#include "midi_control.h"

#include <synth/message/message.h>

#include <cmath>

namespace midi {

constexpr const uint8_t note_off = 0x80;
constexpr const uint8_t note_on = 0x90;

} // namespace midi

MidiControl::MidiControl(Transceiver &transceiver)
    : transceiver_{transceiver} {}

void MidiControl::handle(MidiPacket const &packet) {
  auto const message = packet[1] & 0xF0;
  auto const channel = packet[1] & 0x0F;

  switch (message) {
  case midi::note_off:
    release(channel);
    break;

  case midi::note_on:
    play(channel, packet[2] & 0x7F);
    break;

  default:
    break;
  }
}

void MidiControl::play(uint8_t channel, uint8_t note) {
  auto const frequency =
      static_cast<uint16_t>(powf(2, (note - 69) / 12.0) * 440);

  Message set_frequency{
      .address = channel,
      .command = SetRegister{.reg = Register::Frequency, .value = frequency}};
  send(set_frequency, [this](Word data) { transceiver_.send(data); });

  Message trigger{.address = channel, .command = Trigger{}};
  send(trigger, [this](Word data) { transceiver_.send(data); });
}

void MidiControl::release(uint8_t channel) {
  Message release{.address = channel, .command = Release{}};
  send(release, [this](Word data) { transceiver_.send(data); });
}
