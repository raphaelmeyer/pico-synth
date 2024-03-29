#include "synth_control.h"

#include "focus.h"

#include <synth/message/message.h>

namespace {

Command get_command(Property property, Channel const &channel) {
  switch (property) {
  case Property::Volume:
    return SetVolume{.volume = channel.volume};

  case Property::Attack:
    return SetAttack{.attack = channel.attack};

  case Property::Decay:
    return SetDecay{.decay = channel.decay};

  case Property::Sustain:
    return SetSustain{.sustain = channel.sustain};

  case Property::Release:
    return SetRelease{.release = channel.release};

  case Property::WaveForm:
    return SetWaveForm{.wave = channel.wave};
  }

  return {};
}

} // namespace

SynthControl::SynthControl(Model const &model, Focus const &focus,
                           Sender &sender)
    : model_{model}, focus_{focus}, sender_{sender} {}

void SynthControl::handle(ControlEvent event) {
  switch (event) {
  default:
    break;

  case ControlEvent::Change:
    update_parameter();
    break;
  }
}

void SynthControl::update_parameter() {
  auto const &parameter = focus_.focused();
  uint8_t const channel = parameter.oscillator;

  auto const command =
      get_command(parameter.property, model_.channels.at(channel));

  if (not std::holds_alternative<std::monostate>(command)) {
    send_message(Message{.address = channel, .command = command}, sender_);
  }
}
