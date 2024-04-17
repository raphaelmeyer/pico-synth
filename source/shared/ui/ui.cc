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
      wave_{"Wave", Left, Row_1}, volume_{LV_SYMBOL_VOLUME_MAX, Right, Row_1},
      attack_{"Attack", Left, Row_2}, decay_{"Decay", Right, Row_2},
      sustain_{"Sustain", Left, Row_3}, release_{"Release", Right, Row_3} {}

void UI::show() {
  lv_style_init(&style_);
  lv_style_set_bg_opa(&style_, LV_OPA_COVER);
  lv_style_set_bg_color(&style_, theme::bg_color);
  lv_style_set_text_color(&style_, theme::text_color);
  lv_obj_add_style(lv_scr_act(), &style_, 0);

  wave_.show();

  oscillator_.show();
  volume_.show();
  attack_.show();
  decay_.show();
  sustain_.show();
  release_.show();

  oscillator_.select(focus_.focused().oscillator);

  auto const channel = model_.channels.at(focus_.focused().oscillator);

  wave_.select(channel.wave);
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
    wave_.focus();
    break;
  }
}

void UI::handle(ControlEvent event) {
  auto const property = focus_.focused().property;

  switch (event) {
  case ControlEvent::Focus:
    oscillator_.select(focus_.focused().oscillator);
    if (property == Property::Volume) {
      volume_.focus();
    } else {
      volume_.blur();
    }
    if (property == Property::Attack) {
      attack_.focus();
    } else {
      attack_.blur();
    }
    if (property == Property::Decay) {
      decay_.focus();
    } else {
      decay_.blur();
    }
    if (property == Property::Sustain) {
      sustain_.focus();
    } else {
      sustain_.blur();
    }
    if (property == Property::Release) {
      release_.focus();
    } else {
      release_.blur();
    }
    if (property == Property::WaveForm) {
      wave_.focus();
    } else {
      wave_.blur();
    }
    break;

  case ControlEvent::Edit:
    switch (property) {
    case Property::Volume:
      volume_.edit();
      break;
    case Property::Attack:
      attack_.edit();
      break;
    case Property::Decay:
      decay_.edit();
      break;
    case Property::Sustain:
      sustain_.edit();
      break;
    case Property::Release:
      release_.edit();
      break;
    case Property::WaveForm:
      wave_.edit();
      break;
    }
    break;

  case ControlEvent::Confirm:
    switch (property) {
    case Property::Volume:
      volume_.confirm();
      break;
    case Property::Attack:
      attack_.confirm();
      break;
    case Property::Decay:
      decay_.confirm();
      break;
    case Property::Sustain:
      sustain_.confirm();
      break;
    case Property::Release:
      release_.confirm();
      break;
    case Property::WaveForm:
      wave_.confirm();
      break;
    }
    break;

  case ControlEvent::Change:
    auto const channel = model_.channels.at(focus_.focused().oscillator);
    switch (property) {
    case Property::Volume:
      volume_.set_value(channel.volume);
      break;

    case Property::Attack:
      attack_.set_value(channel.attack);
      break;

    case Property::Decay:
      decay_.set_value(channel.decay);
      break;

    case Property::Sustain:
      sustain_.set_value(channel.sustain);
      break;

    case Property::Release:
      release_.set_value(channel.release);
      break;

    case Property::WaveForm:
      wave_.select(channel.wave);
      break;
    }
    break;
  }
}
