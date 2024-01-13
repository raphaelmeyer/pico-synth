#pragma once

#include "ui/parameter.h"

#include <vector>

struct Index {
  int oscillator;
  Parameter parameter;
};

class Selector {
public:
  Selector(std::initializer_list<int> oscillators);

  Index selected();
  void browse(int steps);

private:
  std::vector<Index> indices_{};
  int selected_{0};
};
