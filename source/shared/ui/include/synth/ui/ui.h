#pragma once

#include "oscillator_selection.h"
#include "range_input.h"
#include "synth/control/model.h"
#include "wave_form_selection.h"

#include <synth/control/control_event.h>

#include <lvgl.h>

class Focus;
struct Model;

class UI {

public:
  UI(Model const &model, Focus const &focus);

  void show();

  void handle(ControlEvent event);

private:
  void update(Property property, std::invocable<Selectable &> auto function) {
    switch (property) {
    case Property::Volume:
      function(volume_);
      break;
    case Property::Attack:
      function(attack_);
      break;
    case Property::Decay:
      function(decay_);
      break;
    case Property::Sustain:
      function(sustain_);
      break;
    case Property::Release:
      function(release_);
      break;
    case Property::WaveForm:
      function(wave_);
      break;
    }
  }

  void update_all(std::invocable<Selectable &, Property> auto function) {
    function(volume_, Property::Volume);
    function(attack_, Property::Attack);
    function(decay_, Property::Decay);
    function(sustain_, Property::Sustain);
    function(release_, Property::Release);
    function(wave_, Property::WaveForm);
  }

  Model const &model_;
  Focus const &focus_;

  OscillatorSelection oscillator_;

  WaveFormSelection wave_;

  RangeInput volume_;
  RangeInput attack_;
  RangeInput decay_;
  RangeInput sustain_;
  RangeInput release_;

  lv_style_t style_{};
};
