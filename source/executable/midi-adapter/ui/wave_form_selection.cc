#include "wave_form_selection.h"

#include "ui/theme.h"

LV_IMG_DECLARE(img_square_wave);
LV_IMG_DECLARE(img_sawtooth_wave);
LV_IMG_DECLARE(img_triangle_wave);
LV_IMG_DECLARE(img_noise_wave);

WaveFormSelection::WaveFormSelection(lv_coord_t y) : y_{y} {}

void WaveFormSelection::show() {
  ui_label_ = lv_label_create(lv_scr_act());
  lv_label_set_text(ui_label_, "Wave");
  lv_obj_set_width(ui_label_, 60);
  lv_obj_set_style_text_align(ui_label_, LV_TEXT_ALIGN_LEFT, 0);
  lv_obj_align(ui_label_, LV_ALIGN_TOP_LEFT, 2, y_);

  lv_obj_set_style_text_color(ui_label_, theme::selected_color,
                              LV_STATE_FOCUSED);
  lv_obj_set_style_text_color(ui_label_, theme::edit_color, LV_STATE_EDITED);

  ui_icon_ = lv_img_create(lv_scr_act());
  lv_obj_align(ui_icon_, LV_ALIGN_TOP_LEFT, 16, y_ + 16);
  lv_obj_set_size(ui_icon_, 16, 12);
  lv_obj_set_style_img_recolor(ui_icon_, theme::text_color, 0);
  lv_obj_set_style_img_recolor_opa(ui_icon_, 255, 0);

  lv_obj_set_style_img_recolor(ui_icon_, theme::selected_color,
                               LV_STATE_FOCUSED);
  lv_obj_set_style_img_recolor(ui_icon_, theme::edit_color, LV_STATE_EDITED);

  lv_img_set_src(ui_icon_, &img_square_wave);
}

void WaveFormSelection::select(WaveForm wave_form) {
  switch (wave_form) {
  case WaveForm::Square:
    lv_img_set_src(ui_icon_, &img_square_wave);
    break;

  case WaveForm::Triangle:
    lv_img_set_src(ui_icon_, &img_triangle_wave);
    break;

  case WaveForm::Sawtooth:
    lv_img_set_src(ui_icon_, &img_sawtooth_wave);
    break;

  case WaveForm::Noise:
    lv_img_set_src(ui_icon_, &img_noise_wave);
    break;
  }
}

void WaveFormSelection::focus() {
  lv_obj_add_state(ui_label_, LV_STATE_FOCUSED);
  lv_obj_add_state(ui_icon_, LV_STATE_FOCUSED);
}

void WaveFormSelection::blur() {
  lv_obj_clear_state(ui_label_, LV_STATE_FOCUSED);
  lv_obj_clear_state(ui_icon_, LV_STATE_FOCUSED);
}

void WaveFormSelection::edit() {
  lv_obj_add_state(ui_label_, LV_STATE_EDITED);
  lv_obj_add_state(ui_icon_, LV_STATE_EDITED);
}

void WaveFormSelection::confirm() {
  lv_obj_clear_state(ui_label_, LV_STATE_EDITED);
  lv_obj_clear_state(ui_icon_, LV_STATE_EDITED);
}
