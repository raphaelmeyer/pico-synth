#include "midi_control.h"

#include <synth/message/message.h>

#include <cmath>

namespace midi {

constexpr const uint8_t note_off = 0x80;
constexpr const uint8_t note_on = 0x90;

} // namespace midi

MidiControl::MidiControl(Sender &sender) : sender_{sender} {}

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

  Message set_frequency{.address = channel,
                        .command = SetFrequency{.frequency = frequency}};
  send_message(set_frequency, sender_);

  Message trigger{.address = channel, .command = Trigger{}};
  send_message(trigger, sender_);
}

void MidiControl::release(uint8_t channel) {
  Message release{.address = channel, .command = Release{}};
  send_message(release, sender_);
}
