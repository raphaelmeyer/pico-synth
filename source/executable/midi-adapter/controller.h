#pragma once

#include "selector.h"
#include "ui/parameter.h"
#include "ui/wave_form_selection.h"

#include <map>

class Knob;
class UI;
class Synth;

class Controller {
public:
  Controller(Knob &knob, UI &ui, Synth &synth,
             std::initializer_list<int> oscillators);

  void init();
  void task();

private:
  void update_selection(int steps);
  void update_parameter(int steps);
  void refresh_ui();
  void update_synth(Parameter parameter, int oscillator);

  enum class State { Select, Edit };

  Knob &knob_;
  UI &ui_;
  Synth &synth_;

  Selector selector_;

  State state_{State::Select};

  struct Parameters {
    WaveForm wave_form{};
    std::map<Parameter, uint16_t> values{};
  };

  std::map<int, Parameters> parameters_{};
};
