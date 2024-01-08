#include "range_input.h"

#include "theme.h"

RangeInput::RangeInput(std::string label, lv_coord_t x, lv_coord_t y)
    : label_{label}, x_{x}, y_{y} {}

void RangeInput::show() {
  ui_label_ = lv_label_create(lv_scr_act());
  lv_label_set_text(ui_label_, label_.c_str());
  lv_obj_set_width(ui_label_, 60);
  lv_obj_set_style_text_align(ui_label_, LV_TEXT_ALIGN_LEFT, 0);
  lv_obj_align(ui_label_, LV_ALIGN_TOP_LEFT, x_ + 2, y_);

  // lv_obj_set_style_border_width(ui_label_, 1, 0);
  // lv_obj_set_style_border_color(ui_label_, theme::text_color, 0);

  ui_value_ = lv_label_create(lv_scr_act());
  lv_label_set_text(ui_value_, "12345");
  lv_obj_set_width(ui_value_, 60);
  lv_obj_set_style_text_align(ui_value_, LV_TEXT_ALIGN_RIGHT, 0);
  lv_obj_align(ui_value_, LV_ALIGN_TOP_LEFT, x_ + 2, y_ + 16);

  // lv_obj_set_style_border_width(ui_value_, 1, 0);
  // lv_obj_set_style_border_color(ui_value_, theme::text_color, 0);
}
