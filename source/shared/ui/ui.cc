#include "ui.h"
#include "theme.h"

#include <synth/control/focus.h>
#include <synth/control/model.h>

#include <string>

namespace {

// constexpr const lv_coord_t Left = 0;
constexpr const lv_coord_t Right = 64;

constexpr const lv_coord_t Row_0 = 2;
constexpr const lv_coord_t Row_1 = 22;
// constexpr const lv_coord_t Row_2 = 58;
// constexpr const lv_coord_t Row_3 = 94;

} // namespace

UI::UI(Model const &model, Focus const &focus)
    : model_{model}, focus_{focus}, oscillator_(Row_0) {}

void UI::show() {
  lv_style_init(&style_);
  lv_style_set_bg_opa(&style_, LV_OPA_COVER);
  lv_style_set_bg_color(&style_, theme::bg_color);
  lv_style_set_text_color(&style_, theme::text_color);
  lv_obj_add_style(lv_scr_act(), &style_, 0);

  oscillator_.show();

  oscillator_.select(focus_.focused().oscillator);

  // volume
  lv_coord_t x_ = Right;
  lv_coord_t y_ = Row_1;
  std::string label_{LV_SYMBOL_VOLUME_MAX};

  auto ui_label_ = lv_label_create(lv_scr_act());
  lv_label_set_text(ui_label_, label_.c_str());
  lv_obj_set_width(ui_label_, 60);
  lv_obj_set_style_text_align(ui_label_, LV_TEXT_ALIGN_LEFT, 0);
  lv_obj_align(ui_label_, LV_ALIGN_TOP_LEFT, x_ + 2, y_);

  lv_obj_set_style_text_color(ui_label_, theme::selected_color,
                              LV_STATE_FOCUSED);
  lv_obj_set_style_text_color(ui_label_, theme::edit_color, LV_STATE_EDITED);

  auto ui_value_ = lv_label_create(lv_scr_act());
  lv_label_set_text(ui_value_, "-----");
  lv_obj_set_width(ui_value_, 60);
  lv_obj_set_style_text_align(ui_value_, LV_TEXT_ALIGN_RIGHT, 0);
  lv_obj_align(ui_value_, LV_ALIGN_TOP_LEFT, x_ + 2, y_ + 16);

  lv_obj_set_style_text_color(ui_value_, theme::selected_color,
                              LV_STATE_FOCUSED);
  lv_obj_set_style_text_color(ui_value_, theme::edit_color, LV_STATE_EDITED);

  lv_label_set_text_fmt(ui_value_, "%u",
                        model_.channels.at(focus_.focused().oscillator).volume);
}
