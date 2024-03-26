#include "control.h"

#include "control_event.h"
#include "focus.h"
#include "model.h"

namespace {

uint16_t change_parameter_value(int steps, uint16_t parameter) {
  auto const log_steps = (steps > 2)
                             ? 1 << (steps * 2)
                             : ((steps < -2) ? -(1 << (-steps * 2)) : steps);
  return std::min(std::max(0, parameter + log_steps), 65535);
}

WaveForm change_wave_form(int steps, WaveForm wave) {
  constexpr int const NumberOfWaveForms = 4;

  auto const index = (steps + static_cast<int>(wave)) % NumberOfWaveForms;
  switch (index) {
  default:
    return WaveForm::Noise;

  case 1:
  case -3:
    return WaveForm::Square;

  case 2:
  case -2:
    return WaveForm::Triangle;

  case 3:
  case -1:
    return WaveForm::Sawtooth;
  }
}

} // namespace

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Control::Control(Model &model, Focus &focus) : model_{model}, focus_{focus} {}

void Control::handle(InputEvent event) {
  std::visit(overloaded{

                 [this](Click) {
                   if (focus_.edited()) {
                     focus_.confirm();
                     for (auto notify : listeners_) {
                       notify(ControlEvent::Confirm);
                     }
                   } else {
                     focus_.edit();
                     for (auto notify : listeners_) {
                       notify(ControlEvent::Edit);
                     }
                   }
                 },

                 [this](Rotate rotate) {
                   if (focus_.edited()) {
                     change_value(rotate.diff);
                     for (auto notify : listeners_) {
                       notify(ControlEvent::Change);
                     }
                   } else {
                     change_selection(rotate.diff);
                     for (auto notify : listeners_) {
                       notify(ControlEvent::Focus);
                     }
                   }
                 }

             },
             event);
}

void Control::onEvent(std::function<void(ControlEvent)> listener) {
  listeners_.push_back(listener);
}

void Control::change_value(int diff) {
  auto const focused = focus_.focused();
  auto &channel = model_.channels.at(focused.oscillator);
  switch (focused.property) {
  case Property::Volume:
    channel.volume = change_parameter_value(diff, channel.volume);
    break;

  case Property::Attack:
    channel.attack = change_parameter_value(diff, channel.attack);
    break;

  case Property::Decay:
    channel.decay = change_parameter_value(diff, channel.decay);
    break;

  case Property::Sustain:
    channel.sustain = change_parameter_value(diff, channel.sustain);
    break;

  case Property::Release:
    channel.release = change_parameter_value(diff, channel.release);
    break;

  case Property::WaveForm:
    channel.wave = change_wave_form(diff, channel.wave);
    break;
  }
}

void Control::change_selection(int diff) { focus_.scroll(diff); }
