#include "controller.h"

#include "knob.h"
#include "ui/parameter.h"
#include "ui/ui.h"
#include "ui/wave_form_selection.h"

#include <variant>

namespace {

WaveForm change_wave_form(WaveForm previous, int steps) {
  constexpr int const NumberOfWaveForms = 4;

  auto const index = (steps + static_cast<int>(previous)) % NumberOfWaveForms;
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

Controller::Controller(Knob &knob, UI &ui,
                       std::initializer_list<int> oscillators)
    : knob_{knob}, ui_{ui}, selector_{oscillators} {

  for (auto const id : oscillators) {
    parameters_.insert({id, {}});
  }
}

void Controller::task() {
  auto const event = knob_.event();

  if (std::holds_alternative<Knob::Push>(event)) {
    if (state_ == State::Select) {
      ui_.edit(Parameter::Attack);
      state_ = State::Edit;
    } else if (state_ == State::Edit) {
      ui_.confirm(Parameter::Attack);
      state_ = State::Select;
    }
  }

  if (auto rotate = std::get_if<Knob::Rotate>(&event); rotate != nullptr) {
    if (state_ == State::Select) {
      update_selection(rotate->steps);
    } else if (state_ == State::Edit) {
      update_parameter(rotate->steps);
    }
  }
}

void Controller::update_selection(int steps) {
  auto const previous = selector_.selected();
  selector_.browse(steps);
  auto const updated = selector_.selected();

  if (previous.oscillator != updated.oscillator) {
    ui_.select_oscillator(updated.oscillator);
    ui_.select_wave_form(parameters_.at(updated.oscillator).wave_form);

    // TODO
    // for (param : params) { ui_.set_value(param, ...);
  }

  if (previous.parameter != updated.parameter) {
    ui_.focus(updated.parameter);
  }
}

void Controller::update_parameter(int steps) {
  auto const index = selector_.selected();
  if (index.parameter == Parameter::WaveForm) {
    auto const previous = parameters_.at(index.oscillator).wave_form;
    auto const updated = change_wave_form(previous, steps);
    if (previous != updated) {
      parameters_.at(index.oscillator).wave_form = updated;
      ui_.select_wave_form(updated);
    }
  } else {
    // TODO: 'log' adjust steps

    auto const previous =
        parameters_.at(index.oscillator).values.at(index.parameter);

    uint16_t const updated = std::min(std::max(0, previous + steps), 65535);
    parameters_.at(index.oscillator).values[index.parameter] = updated;

    ui_.set_value(index.parameter, updated);
  }
}
