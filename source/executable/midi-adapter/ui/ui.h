#pragma once

#include "oscillator_selection.h"
#include "range_input.h"
#include "wave_form_selection.h"

#include <lvgl.h>

class UI {
public:
  UI();

  void show();

  void select_oscillator(int id);
  void select_wave_form(WaveForm wave_form);

private:
  OscillatorSelection oscillator_;
  WaveFormSelection wave_form_;
  RangeInput volume_;
  RangeInput attack_;
  RangeInput decay_;
  RangeInput sustain_;
  RangeInput release_;

  lv_style_t style_{};
};
