#pragma once

#include <variant>

struct Focused {
  // channel, item
};
struct Edited {
  // channel, item
};
struct Confirmed {
  // channel, item (, value )
};
struct Changed {
  // channel, item, diff
};

using ControlEvent = std::variant<Focused, Edited, Confirmed, Changed>;
