#include "synth.h"

#include <synth/message/message.h>

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Synth::Synth(uint32_t sampling_rate, Random &random)
    : voices_{Voice{sampling_rate, random}, Voice{sampling_rate, random},
              Voice{sampling_rate, random}, Voice{sampling_rate, random}} {}

void Synth::handle(Message const &message) {
  std::size_t channel = message.address & 0x0F;
  if (channel >= voices_.size()) {
    return;
  }

  auto &voice = voices_.at(channel);

  std::visit(
      overloaded{

          [&voice](Trigger const &) { voice.envelope_generator.trigger(); },

          [&voice](Release const &) { voice.envelope_generator.release(); },

          [this, &voice](SetRegister const &set) { set_value(voice, set); },

          [&voice](SetWaveForm const &set) {
            voice.oscillator.set_type(set.wave);
          }

      },
      message.command);
}

Sample Synth::next_sample() {
  uint32_t sample = 0;
  for (auto &voice : voices_) {
    sample += voice.envelope_generator.next_value();
  }
  uint16_t const clipped = std::max(sample, static_cast<uint32_t>(65535));
  return {clipped, clipped};
}

void Synth::set_value(Voice &voice, SetRegister const &set) {
  switch (set.reg) {
  default:
    break;

  case Register::Frequency:
    voice.oscillator.set_frequency(set.value);
    break;

  case Register::Attack:
    voice.envelope_generator.set_attack(set.value);
    break;

  case Register::Decay:
    voice.envelope_generator.set_decay(set.value);
    break;

  case Register::Sustain:
    voice.envelope_generator.set_sustain(set.value);
    break;

  case Register::Release:
    voice.envelope_generator.set_release(set.value);
    break;

  case Register::Volume:
    voice.envelope_generator.set_volume(set.value);
    break;
  }
}
