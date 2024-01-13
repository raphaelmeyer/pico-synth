#pragma once

#include "ui/input_field.h"

#include <lvgl.h>

#include <string>

class RangeInput : public InputField {
public:
  RangeInput(std::string label, lv_coord_t x, lv_coord_t y);

  void show();

  void set_value(uint16_t value);

  void focus() override;
  void blur() override;

  void edit() override;
  void confirm() override;

private:
  std::string const label_;
  lv_coord_t x_;
  lv_coord_t y_;

  lv_obj_t *ui_label_{nullptr};
  lv_obj_t *ui_value_{nullptr};
};
