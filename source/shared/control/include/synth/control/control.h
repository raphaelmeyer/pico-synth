#pragma once

#include "control_event.h"
#include "input_event.h"

#include <functional>

class Focus;
struct Model;

class Control {
public:
  Control(Model &model, Focus &focus);

  void handle(InputEvent event);
  void onEvent(std::function<void(ControlEvent)> listener);

private:
  void change_value(int diff);
  void change_selection(int diff);

  Model &model_;
  Focus &focus_;

  std::vector<std::function<void(ControlEvent)>> listeners_{};
};
