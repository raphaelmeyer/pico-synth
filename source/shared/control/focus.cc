#include "focus.h"

Focus::Focus() {
  for (std::size_t oscillator = 0; oscillator < NumOscillators; ++oscillator) {
    for (auto property :
         {Property::WaveForm, Property::Volume, Property::Attack,
          Property::Decay, Property::Sustain, Property::Release}) {
      order_.emplace_back(oscillator, property);
    }
  }
}

Parameter Focus::focused() const { return order_.at(focused_); }

bool Focus::edited() const { return edited_; }

void Focus::edit() { edited_ = true; }

void Focus::confirm() { edited_ = false; }

void Focus::scroll(int steps) {
  edited_ = false;
  auto updated =
      (static_cast<int>(focused_) + steps) % static_cast<int>(order_.size());
  if (updated < 0) {
    updated += order_.size();
  }
  focused_ = updated;
}
