#include "synth.h"

#include <synth/message/message.h>
#include <variant>

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

          [&voice](SetFrequency const &set) {
            voice.oscillator.set_frequency(set.frequency);
          },

          [&voice](SetVolume const &set) {
            voice.envelope_generator.set_volume(set.volume);
          },

          [&voice](SetAttack const &set) {
            voice.envelope_generator.set_attack(set.attack);
          },

          [&voice](SetDecay const &set) {
            voice.envelope_generator.set_decay(set.decay);
          },

          [&voice](SetSustain const &set) {
            voice.envelope_generator.set_sustain(set.sustain);
          },

          [&voice](SetRelease const &set) {
            voice.envelope_generator.set_release(set.release);
          },

          [&voice](SetWaveForm const &set) {
            voice.oscillator.set_type(set.wave);
          },

          [](std::monostate) {}

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
