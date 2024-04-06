#pragma once

#include <synth/definitions.h>

#include <lvgl.h>

#include <array>

class OscillatorSelection {
public:
  OscillatorSelection(lv_coord_t y);

  void show();

  void select(OscillatorId id);

private:
  lv_coord_t const y_;

  OscillatorId selected_{};

  lv_obj_t *ui_label_{nullptr};
  std::array<lv_obj_t *, NumOscillators> ui_items_{};
  lv_style_t style_selected_{};
};
