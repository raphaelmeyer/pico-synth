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

    control.handle(Rotate{-3});

    REQUIRE(spy.size() == 1);
    REQUIRE(spy.at(0) == ControlEvent::Focus);
    REQUIRE(focus.focused().oscillator == 3);
    REQUIRE(focus.focused().property == Property::Decay);
  }

  SUBCASE("should notify when parameter is edited") {
    std::vector<ControlEvent> spy;

    Control control{model, focus};
    control.onEvent([&spy](ControlEvent event) { spy.push_back(event); });

    control.handle(Rotate{2});
    control.handle(Click{});

    REQUIRE(spy.size() == 2);
    REQUIRE(spy.at(1) == ControlEvent::Edit);
    REQUIRE(focus.focused().oscillator == 0);
    REQUIRE(focus.focused().property == Property::Attack);
    REQUIRE(focus.edited());
  }

  SUBCASE("should notify when parameter is changed") {
    std::vector<ControlEvent> spy;
    model.channels.at(1).volume = 123;

    Control control{model, focus};
    control.onEvent([&spy](ControlEvent event) { spy.push_back(event); });

    control.handle(Rotate{7});
    control.handle(Click{});
    control.handle(Rotate{-2});

    REQUIRE(spy.size() == 3);
    REQUIRE(spy.at(2) == ControlEvent::Change);
    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Volume);
    REQUIRE(focus.edited());
    REQUIRE(model.channels.at(1).volume == 121);
  }

  SUBCASE("should notify when parameter is confirmed") {
    std::vector<ControlEvent> spy;
    model.channels.at(1).sustain = 1000;

    Control control{model, focus};
    control.onEvent([&spy](ControlEvent event) { spy.push_back(event); });

    control.handle(Rotate{10});
    control.handle(Click{});
    control.handle(Rotate{1});
    control.handle(Click{});

    REQUIRE(spy.size() == 4);
    REQUIRE(spy.at(3) == ControlEvent::Confirm);
    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Sustain);
    REQUIRE(model.channels.at(1).sustain == 1001);
    REQUIRE(not focus.edited());
  }
}

} // namespace
