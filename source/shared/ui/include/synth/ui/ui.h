#pragma once

#include "oscillator_selection.h"
#include "range_input.h"
#include "wave_form_selection.h"

#include <synth/control/control_event.h>

#include <lvgl.h>

class Focus;
struct Model;

class UI {

public:
  UI(Model const &model, Focus const &focus);

  void show();

  void handle(ControlEvent event);

private:
  Model const &model_;
  Focus const &focus_;

  OscillatorSelection oscillator_;

  WaveFormSelection wave_;

  RangeInput volume_;
  RangeInput attack_;
  RangeInput decay_;
  RangeInput sustain_;
  RangeInput release_;

  lv_style_t style_{};
};
