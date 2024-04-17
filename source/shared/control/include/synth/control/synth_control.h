#pragma once

#include "control_event.h"

class Focus;
struct Model;
class Sender;

class SynthControl {
public:
  SynthControl(Model const &model, Focus const &focus, Sender &sender);

  void handle(ControlEvent event);

private:
  void update_parameter();

  Model const &model_;
  Focus const &focus_;
  Sender &sender_;
};
