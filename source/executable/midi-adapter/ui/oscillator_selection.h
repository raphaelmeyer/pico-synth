#pragma once

#include <lvgl.h>

#include <map>

class OscillatorSelection {
public:
  OscillatorSelection(lv_coord_t y, int num_oscillators);

  void show();

  void select(int id);

private:
  lv_coord_t const y_;
  int const num_oscillators_;

  int selected_{};

  lv_obj_t *ui_label_{nullptr};
  std::map<int32_t, lv_obj_t *> ui_items_{};
  lv_style_t style_selected_{};
};
