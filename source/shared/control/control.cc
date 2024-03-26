#include "control.h"

#include "control_event.h"
#include "focus.h"
#include "model.h"

namespace {

void change_parameter_value(int steps, uint16_t &parameter) {
  if (steps > 2) {
    parameter += 1 << (steps * 2);
  } else if (steps < -2) {
    parameter -= 1 << (-steps * 2);
  } else {
    parameter += steps;
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
    change_parameter_value(diff, channel.volume);
    break;

  case Property::Attack:
    change_parameter_value(diff, channel.attack);
    break;

  case Property::Decay:
    change_parameter_value(diff, channel.decay);
    break;

  case Property::Sustain:
    change_parameter_value(diff, channel.sustain);
    break;

  case Property::Release:
    change_parameter_value(diff, channel.release);
    break;

  case Property::WaveForm:
    break;
  }
}

void Control::change_selection(int diff) { focus_.scroll(diff); }
