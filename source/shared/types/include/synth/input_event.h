#pragma once

#include <variant>

struct Rotate {
  int diff;
};

struct Click {};

using InputEvent = std::variant<Rotate, Click>;
