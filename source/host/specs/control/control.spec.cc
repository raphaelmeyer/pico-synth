#include <doctest/doctest.h>

#include <synth/control/control.h>
#include <synth/control/control_event.h>
#include <synth/control/focus.h>
#include <synth/control/input_event.h>
#include <synth/control/model.h>

namespace {

TEST_CASE("control") {
  Model model{};
  Focus focus{};

  SUBCASE("should focus parameter on rotate event") {
    Control control{model, focus};

    control.handle(Rotate{13});

    REQUIRE(focus.focused().oscillator == 2);
    REQUIRE(focus.focused().property == Property::Volume);

    control.handle(Rotate{-3});

    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Sustain);
  }

  SUBCASE("should notify when parameter is focused") {
    std::vector<ControlEvent> spy;

    Control control{model, focus};
    control.onEvent([&spy](ControlEvent event) { spy.push_back(event); });

    control.handle(Rotate{2});

    REQUIRE(spy.size() == 1);
    REQUIRE(std::holds_alternative<Focused>(spy.at(0)));
  }
}

} // namespace
