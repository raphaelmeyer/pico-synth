#include "oscillator_selection.h"

#include "theme.h"
#include <src/core/lv_obj.h>

OscillatorSelection::OscillatorSelection(lv_coord_t y, int num_oscillators)
    : y_{y}, num_oscillators_{num_oscillators} {}

void OscillatorSelection::show() {
  lv_style_init(&style_selected_);
  lv_style_set_outline_color(&style_selected_, theme::text_color);
  lv_style_set_outline_width(&style_selected_, 1);
  lv_style_set_outline_pad(&style_selected_, 1);

  ui_label_ = lv_label_create(lv_scr_act());
  lv_label_set_text(ui_label_, LV_SYMBOL_AUDIO);
  lv_obj_set_style_text_align(ui_label_, LV_TEXT_ALIGN_LEFT, 0);
  lv_obj_align(ui_label_, LV_ALIGN_TOP_LEFT, 2, y_);

  for (int id = 1; id <= num_oscillators_; ++id) {
    auto const item = lv_label_create(lv_scr_act());
    lv_label_set_text_fmt(item, "%d", id);
    lv_obj_set_width(item, 18);
    lv_obj_set_style_text_align(item, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(item, LV_ALIGN_TOP_LEFT, 20 * id + 4, y_);
    ui_items_[id] = item;

    lv_obj_add_style(item, &style_selected_, LV_STATE_CHECKED);
  }
}

void OscillatorSelection::select(int id) {
  auto const previous = ui_items_.find(selected_);
  if (previous != ui_items_.end()) {
    lv_obj_clear_state(previous->second, LV_STATE_CHECKED);
  }

  auto const selected = ui_items_.find(id);
  if (selected != ui_items_.end()) {
    lv_obj_add_state(selected->second, LV_STATE_CHECKED);
    selected_ = id;
  }
}
