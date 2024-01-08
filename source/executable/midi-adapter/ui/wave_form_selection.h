#pragma once

#include <lvgl.h>

enum class WaveForm { Square, Triangle, Sawtooth, Noise };

class WaveFormSelection {

public:
  WaveFormSelection(lv_coord_t y);

  void show();

  void select(WaveForm wave_form);

private:
  lv_coord_t const y_;

  lv_obj_t *ui_label_{nullptr};
  lv_obj_t *ui_icon_{nullptr};
};
