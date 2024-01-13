#pragma once

#include "selector.h"
#include "ui/wave_form_selection.h"

#include <map>

class Knob;
class UI;

class Controller {
public:
  Controller(Knob &knob, UI &ui, std::initializer_list<int> oscillators);

  void task();

private:
  void update_selection(int steps);
  void update_parameter(int steps);

  enum class State { Select, Edit };

  Knob &knob_;
  UI &ui_;

  Selector selector_;

  State state_{State::Select};

  struct Parameters {
    WaveForm wave_form{};
    std::map<Parameter, uint16_t> values{};
  };

  std::map<int, Parameters> parameters_{};
};
