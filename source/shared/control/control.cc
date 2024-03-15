#include "control.h"

#include "focus.h"
#include "model.h"

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
                       notify(Confirm{});
                     }
                   } else {
                     focus_.edit();
                     for (auto notify : listeners_) {
                       notify(Edit{});
                     }
                   }
                 },

                 [this](Rotate rotate) {
                   if (focus_.edited()) {
                     change_value(rotate.diff);
                     for (auto notify : listeners_) {
                       notify(Change{});
                     }
                   } else {
                     change_selection(rotate.diff);
                     for (auto notify : listeners_) {
                       notify(Select{});
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
  if (focused.property == Property::Volume) {
    model_.channels.at(focused.oscillator).volume += diff;
  }
}

void Control::change_selection(int diff) { focus_.scroll(diff); }
