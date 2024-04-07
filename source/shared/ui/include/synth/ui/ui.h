#pragma once

#include "oscillator_selection.h"
#include "range_input.h"

#include <lvgl.h>

class Focus;
struct Model;

class UI {

public:
  UI(Model const &model, Focus const &focus);

  void show();

private:
  Model const &model_;
  Focus const &focus_;

  OscillatorSelection oscillator_;

  RangeInput volume_;
  RangeInput attack_;
  RangeInput decay_;
  RangeInput sustain_;
  RangeInput release_;

  lv_style_t style_{};
};
