#pragma once

#include "control_event.h"
#include "input_event.h"

#include <synth/definitions.h>

#include <functional>
#include <optional>

struct Model;

class Control {
public:
  Control(Model &model);

  void handle(InputEvent event);
  void onEvent(std::function<void(ControlEvent)> listener);

private:
  Model &model_;

  std::vector<std::function<void(ControlEvent)>> listeners_{};
  std::optional<OscillatorId> selected_{};
};
