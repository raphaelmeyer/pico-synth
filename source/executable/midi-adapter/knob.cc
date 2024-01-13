#include "knob.h"

#include "push_button.h"
#include "rotary_encoder.h"

#include <variant>

void Knob::init() { queue_init(&events_, sizeof(Event), 5); }

void Knob::task() {
  auto const now = time_us_64();
  if (now - last > 100'000ull) {
    last = now;

    if (confirm_.pushed()) {
      Event const event = Push{};
      queue_add_blocking(&events_, &event);
    }

    auto const steps = select_.steps();
    if (steps != 0) {
      Event const event = Rotate{steps};
      queue_add_blocking(&events_, &event);
    }
  }
}

Knob::Event Knob::event() {
  Event event{};
  if (queue_try_remove(&events_, &event)) {
    return event;
  }

  return std::monostate{};
}
