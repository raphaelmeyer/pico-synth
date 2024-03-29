#include "controller.h"

#include "knob.h"
#include "message.h"
#include "synth.h"
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

uint16_t encode_wave_form(WaveForm form) {
  switch (form) {
  default:
  case WaveForm::Noise:
    return 0;

  case WaveForm::Square:
    return 1;

  case WaveForm::Triangle:
    return 2;

  case WaveForm::Sawtooth:
    return 3;
  }
}

} // namespace

Controller::Controller(Knob &knob, UI &ui, Synth &synth,
                       std::initializer_list<int> oscillators)
    : knob_{knob}, ui_{ui}, synth_{synth}, selector_{oscillators} {

  for (auto const id : oscillators) {
    parameters_.insert({id,
                        {WaveForm::Square,
                         {{Parameter::Volume, 16384},
                          {Parameter::Attack, 100},
                          {Parameter::Decay, 200},
                          {Parameter::Sustain, 16384},
                          {Parameter::Release, 1024}}}});
  }
}

void Controller::init() {
  auto const selected = selector_.selected();

  for (auto const &oscillator : parameters_) {
    for (auto const parameter :
         {Parameter::WaveForm, Parameter::Volume, Parameter::Attack,
          Parameter::Decay, Parameter::Sustain, Parameter::Release}) {
      update_synth(parameter, oscillator.first);
    }
  }

  ui_.select_oscillator(selected.oscillator);
  ui_.select_wave_form(parameters_[selected.oscillator].wave_form);

  for (auto const parameter :
       {Parameter::Volume, Parameter::Attack, Parameter::Decay,
        Parameter::Sustain, Parameter::Release}) {
    ui_.set_value(parameter,
                  parameters_[selected.oscillator].values[parameter]);
  }

  ui_.focus(selected.parameter);
}

void Controller::task() {
  auto const event = knob_.event();

  if (std::holds_alternative<Knob::Push>(event)) {
    auto const index = selector_.selected();

    if (state_ == State::Select) {
      ui_.edit(index.parameter);
      state_ = State::Edit;
    } else if (state_ == State::Edit) {
      ui_.confirm(index.parameter);
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
    ui_.select_wave_form(parameters_[updated.oscillator].wave_form);

    for (auto const parameter :
         {Parameter::Volume, Parameter::Attack, Parameter::Decay,
          Parameter::Sustain, Parameter::Release}) {
      ui_.set_value(parameter,
                    parameters_[updated.oscillator].values[parameter]);
    }
  }

  if (previous.parameter != updated.parameter) {
    ui_.focus(updated.parameter);
  }
}

void Controller::update_parameter(int steps) {
  auto const index = selector_.selected();
  if (index.parameter == Parameter::WaveForm) {
    auto const previous = parameters_[index.oscillator].wave_form;
    auto const updated = change_wave_form(previous, steps);
    if (previous != updated) {
      parameters_[index.oscillator].wave_form = updated;
      ui_.select_wave_form(updated);
    }
  } else {
    auto const previous = parameters_[index.oscillator].values[index.parameter];

    auto const log_steps =
        steps > 2 ? (2 << steps) : (steps < -2 ? (-(2 << (-steps))) : steps);
    uint16_t const updated = std::min(std::max(0, previous + log_steps), 65535);
    parameters_[index.oscillator].values[index.parameter] = updated;

    ui_.set_value(index.parameter, updated);
  }

  update_synth(index.parameter, index.oscillator);
}

void Controller::update_synth(Parameter parameter, int oscillator) {
  auto const channel = static_cast<uint8_t>(oscillator);

  switch (parameter) {
  case Parameter::WaveForm: {
    auto const message = Message{
        .address = channel,
        .command = WriteRegister{
            .reg = Register::Control,
            .data = encode_wave_form(parameters_[oscillator].wave_form)}};
    synth_.send(message);
  } break;

  case Parameter::Volume: {
    auto const message =
        Message{.address = channel,
                .command = WriteRegister{
                    .reg = Register::Volume,
                    .data = parameters_[oscillator].values[parameter]}};

    synth_.send(message);
  } break;

  case Parameter::Attack: {
    auto const message =
        Message{.address = channel,
                .command = WriteRegister{
                    .reg = Register::Attack,
                    .data = parameters_[oscillator].values[parameter]}};

    synth_.send(message);
  } break;

  case Parameter::Decay: {
    auto const message =
        Message{.address = channel,
                .command = WriteRegister{
                    .reg = Register::Decay,
                    .data = parameters_[oscillator].values[parameter]}};

    synth_.send(message);
  } break;

  case Parameter::Sustain: {
    auto const message =
        Message{.address = channel,
                .command = WriteRegister{
                    .reg = Register::Sustain,
                    .data = parameters_[oscillator].values[parameter]}};

    synth_.send(message);
  } break;

  case Parameter::Release: {
    auto const message =
        Message{.address = channel,
                .command = WriteRegister{
                    .reg = Register::Release,
                    .data = parameters_[oscillator].values[parameter]}};

    synth_.send(message);
  } break;
  }
}
