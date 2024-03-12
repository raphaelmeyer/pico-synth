#include "control.h"

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Control::Control(Model &model) : model_(model) {}

void Control::handle(InputEvent event) {
  std::visit(overloaded{

                 [this](Click) {
                   if (selected_.has_value()) {
                     for (auto notify : listeners_) {
                       notify(Confirm{});
                     }
                   } else {
                     for (auto notify : listeners_) {
                       notify(Edit{});
                     }
                   }
                 },

                 [this](Rotate) {
                   if (selected_.has_value()) {
                     for (auto notify : listeners_) {
                       notify(Change{});
                     }
                   } else {
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
