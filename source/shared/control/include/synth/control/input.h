#pragma once

#include "input_event.h"

#include <functional>

// -> move to device? or midi-adapter?

class Input {
public:
  void onEvent(std::function<void(InputEvent)>);
};
