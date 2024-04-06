#pragma once

#include "oscillator_selection.h"

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

  lv_style_t style_{};
};
