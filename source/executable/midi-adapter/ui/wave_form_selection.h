#pragma once

#include "ui/input_field.h"

#include <lvgl.h>

enum class WaveForm { Noise, Square, Triangle, Sawtooth };

class WaveFormSelection : public InputField {

public:
  WaveFormSelection(lv_coord_t y);

  void show();

  void select(WaveForm wave_form);

  void focus() override;
  void blur() override;

  void edit() override;
  void confirm() override;

private:
  lv_coord_t const y_;

  lv_obj_t *ui_label_{nullptr};
  lv_obj_t *ui_icon_{nullptr};
};
