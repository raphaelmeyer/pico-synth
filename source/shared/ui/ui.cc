#include "ui.h"
#include "theme.h"

#include <synth/control/focus.h>
#include <synth/control/model.h>

#include <string>

namespace {

constexpr const lv_coord_t Left = 0;
constexpr const lv_coord_t Right = 64;

constexpr const lv_coord_t Row_0 = 2;
constexpr const lv_coord_t Row_1 = 22;
constexpr const lv_coord_t Row_2 = 58;
constexpr const lv_coord_t Row_3 = 94;

} // namespace

UI::UI(Model const &model, Focus const &focus)
    : model_{model}, focus_{focus}, oscillator_{Row_0},
      volume_{LV_SYMBOL_VOLUME_MAX, Right, Row_1},
      attack_{"Attack", Left, Row_2}, decay_{"Decay", Right, Row_2},
      sustain_{"Sustain", Left, Row_3}, release_{"Release", Right, Row_3} {}

void UI::show() {
  lv_style_init(&style_);
  lv_style_set_bg_opa(&style_, LV_OPA_COVER);
  lv_style_set_bg_color(&style_, theme::bg_color);
  lv_style_set_text_color(&style_, theme::text_color);
  lv_obj_add_style(lv_scr_act(), &style_, 0);

  oscillator_.show();
  volume_.show();
  attack_.show();
  decay_.show();
  sustain_.show();
  release_.show();

  oscillator_.select(focus_.focused().oscillator);

  auto const channel = model_.channels.at(focus_.focused().oscillator);

  volume_.set_value(channel.volume);
  attack_.set_value(channel.attack);
  decay_.set_value(channel.decay);
  sustain_.set_value(channel.sustain);
  release_.set_value(channel.release);

  switch (focus_.focused().property) {
  case Property::Volume:
    volume_.focus();
    break;
  case Property::Attack:
    attack_.focus();
    break;
  case Property::Decay:
    decay_.focus();
    break;
  case Property::Sustain:
    sustain_.focus();
    break;
  case Property::Release:
    release_.focus();
    break;
  case Property::WaveForm:
    break;
  }
}
