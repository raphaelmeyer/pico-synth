#pragma once

#include "oscillator_selection.h"
#include "range_input.h"
#include "ui/parameter.h"
#include "wave_form_selection.h"

#include <lvgl.h>

class UI {
public:
  UI();

  void show();

  void focus(Parameter parameter);
  void edit(Parameter parameter);
  void confirm(Parameter parameter);

  void select_oscillator(int id);
  void select_wave_form(WaveForm wave_form);
  void set_value(Parameter parameter, uint16_t value);

private:
  OscillatorSelection oscillator_;
  WaveFormSelection wave_form_;

  RangeInput volume_;
  RangeInput attack_;
  RangeInput decay_;
  RangeInput sustain_;
  RangeInput release_;

  lv_style_t style_{};

  Parameter selected_{};
};
