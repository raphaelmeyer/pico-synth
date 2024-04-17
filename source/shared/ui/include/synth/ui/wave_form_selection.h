#pragma once

#include <synth/wave-form.h>

#include <lvgl.h>

#include <string>

class WaveFormSelection {
public:
  WaveFormSelection(std::string label, lv_coord_t x, lv_coord_t y);

  void show();

  void select(WaveForm wave);

  void focus();
  void blur();

  void edit();
  void confirm();

private:
  std::string const label_;
  lv_coord_t const x_;
  lv_coord_t const y_;

  lv_obj_t *ui_label_{nullptr};
  lv_obj_t *ui_icon_{nullptr};
};
