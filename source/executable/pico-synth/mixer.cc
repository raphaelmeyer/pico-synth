#include "mixer.h"

#include "channel.h"
#include "i2s.h"

Mixer::Mixer(Channels &channels, I2S &i2s) : channels_{channels}, i2s_{i2s} {}

void Mixer::mix() {
  auto const left = channels_.at(0).tone_generator.next_value() +
                    channels_.at(1).tone_generator.next_value();
  auto const right = channels_.at(2).tone_generator.next_value() +
                     channels_.at(3).tone_generator.next_value();
  i2s_.output_sample(left, right);
}
