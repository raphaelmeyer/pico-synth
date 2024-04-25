#include "oscillator_selection.h"

#include "theme.h"

OscillatorSelection::OscillatorSelection(lv_coord_t y) : y_{y} {}

void OscillatorSelection::show() {
  lv_style_init(&style_selected_);
  lv_style_set_outline_color(&style_selected_, theme::text_color);
  lv_style_set_outline_width(&style_selected_, 1);
  lv_style_set_outline_pad(&style_selected_, 1);

  ui_label_ = lv_label_create(lv_scr_act());
  lv_label_set_text(ui_label_, LV_SYMBOL_AUDIO);
  lv_obj_set_style_text_align(ui_label_, LV_TEXT_ALIGN_LEFT, 0);
  lv_obj_align(ui_label_, LV_ALIGN_TOP_LEFT, 2, y_);

  int id = 1;
  for (auto &item : ui_items_) {
    item = lv_label_create(lv_scr_act());
    lv_label_set_text_fmt(item, "%d", id);
    lv_obj_set_width(item, 18);
    lv_obj_set_style_text_align(item, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(item, LV_ALIGN_TOP_LEFT, 20 * id + 4, y_);
    lv_obj_add_style(item, &style_selected_, LV_STATE_CHECKED);
    ++id;
  }

  lv_obj_add_state(ui_items_[selected_], LV_STATE_CHECKED);
}

SelectedOscillator OscillatorSelection::select(OscillatorId id) {
  if (id == selected_) {
    return SelectedOscillator::Same;
  }

  lv_obj_clear_state(ui_items_[selected_], LV_STATE_CHECKED);
  selected_ = id;
  lv_obj_add_state(ui_items_[selected_], LV_STATE_CHECKED);

  return SelectedOscillator::Changed;
}
