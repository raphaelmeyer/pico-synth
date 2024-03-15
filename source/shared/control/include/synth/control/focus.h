#pragma once

#include "model.h"

#include "synth/definitions.h"

#include <vector>

struct Parameter {
  OscillatorId oscillator;
  Property property;
};

class Focus {
public:
  Focus();

  Parameter focused() const;
  bool edited() const;

  void edit();
  void confirm();
  void scroll(int steps);

private:
  std::vector<Parameter> order_{};
  std::size_t focused_{};
  bool edited_{};
};
