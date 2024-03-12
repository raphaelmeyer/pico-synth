#pragma once

#include <variant>

struct Select {
  // channel, item
};
struct Edit {
  // channel, item
};
struct Confirm {
  // channel, item (, value )
};
struct Change {
  // channel, item, diff
};

using ControlEvent = std::variant<Select, Edit, Confirm, Change>;
