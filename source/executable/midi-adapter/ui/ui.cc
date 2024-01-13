#include "ui.h"

#include "ui/theme.h"
#include "ui/wave_form_selection.h"

namespace {

constexpr const lv_coord_t Left = 0;
constexpr const lv_coord_t Right = 64;

constexpr const lv_coord_t Row_0 = 2;
constexpr const lv_coord_t Row_1 = 22;
constexpr const lv_coord_t Row_2 = 58;
constexpr const lv_coord_t Row_3 = 94;

} // namespace

UI::UI()
    : oscillator_{Row_0, 4}, wave_form_{Row_1}, volume_{LV_SYMBOL_VOLUME_MAX,
                                                        Right, Row_1},
      attack_{"Attack", Left, Row_2}, decay_{"Decay", Right, Row_2},
      sustain_{"Sustain", Left, Row_3}, release_{"Release", Right, Row_3} {}

void UI::show() {
  lv_style_init(&style_);
  lv_style_set_bg_color(&style_, theme::bg_color);
  lv_style_set_text_color(&style_, theme::text_color);
  lv_obj_add_style(lv_scr_act(), &style_, 0);

  oscillator_.show();
  wave_form_.show();
  volume_.show();
  attack_.show();
  decay_.show();
  sustain_.show();
  release_.show();
}

void UI::focus(Parameter parameter) {
  if (auto input = input_field(selected_); input != nullptr) {
    input->blur();
  }

  if (auto input = input_field(parameter); input != nullptr) {
    input->focus();
  }

  selected_ = parameter;
}

void UI::edit(Parameter parameter) {
  if (auto input = input_field(parameter); input != nullptr) {
    input->edit();
  }
}

void UI::confirm(Parameter parameter) {
  if (auto input = input_field(parameter); input != nullptr) {
    input->confirm();
  }
}

void UI::select_oscillator(int id) { oscillator_.select(id); }

void UI::select_wave_form(WaveForm wave_form) { wave_form_.select(wave_form); }

void UI::set_value(Parameter parameter, uint16_t value) {
  switch (parameter) {
  default:
    break;

  case Parameter::Volume:
    volume_.set_value(value);
    break;

  case Parameter::Attack:
    attack_.set_value(value);
    break;

  case Parameter::Decay:
    decay_.set_value(value);
    break;

  case Parameter::Sustain:
    sustain_.set_value(value);
    break;

  case Parameter::Release:
    release_.set_value(value);
    break;
  }
}

InputField *UI::input_field(Parameter parameter) {
  switch (parameter) {
  default:
    return nullptr;
  case Parameter::WaveForm:
    return &wave_form_;
  case Parameter::Volume:
    return &volume_;
  case Parameter::Attack:
    return &attack_;
  case Parameter::Decay:
    return &decay_;
  case Parameter::Sustain:
    return &sustain_;
  case Parameter::Release:
    return &release_;
  }
}
