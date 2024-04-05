#pragma once

#include <lvgl.h>

struct Model;

class UI {

public:
  UI(Model const &model);

  void show();

private:
  Model const &model_;

  lv_style_t style_{};
};
