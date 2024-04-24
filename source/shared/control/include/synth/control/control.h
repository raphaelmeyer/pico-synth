#pragma once

#include "control_event.h"

#include <synth/input_event.h>

#include <functional>

class Focus;
struct Model;

class Control {
public:
  Control(Model &model, Focus &focus,
          std::function<void(ControlEvent)> on_event);

  void handle(InputEvent event);

private:
  void change_value(int diff);
  void change_selection(int diff);

  Model &model_;
  Focus &focus_;

  std::function<void(ControlEvent)> on_event_{};
};
