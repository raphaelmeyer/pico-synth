#include "selector.h"

Selector::Selector(std::initializer_list<int> oscillators) {
  for (auto const id : oscillators) {
    for (Parameter const param :
         {Parameter::WaveForm, Parameter::Volume, Parameter::Attack,
          Parameter::Decay, Parameter::Sustain, Parameter::Release}) {
      indices_.push_back({.oscillator = id, .parameter = param});
    }
  }
}

Index Selector::selected() { return indices_[selected_]; }

void Selector::browse(int steps) {
  auto updated = (selected_ + steps) % static_cast<int>(indices_.size());
  if (updated < 0) {
    updated += indices_.size();
  }

  selected_ = updated;
}
