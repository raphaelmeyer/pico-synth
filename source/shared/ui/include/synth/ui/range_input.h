#pragma once

#include <lvgl.h>

#include <string>

class RangeInput {
public:
  RangeInput(std::string label, lv_coord_t x, lv_coord_t y);

  void show();

  void set_value(uint16_t value);

  void focus();
  void blur();

  void edit();
  void confirm();

private:
  std::string const label_;
  lv_coord_t x_;
  lv_coord_t y_;

  lv_obj_t *ui_label_{nullptr};
  lv_obj_t *ui_value_{nullptr};
};
