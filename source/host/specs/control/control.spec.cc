#include <cstdint>
#include <doctest/doctest.h>

#include <synth/control/control.h>
#include <synth/control/control_event.h>
#include <synth/control/focus.h>
#include <synth/control/input_event.h>
#include <synth/control/model.h>

namespace {

TEST_CASE("focus") {
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

  SUBCASE("should edit focused parameter on click event") {
    Control control{model, focus};
    control.handle(Rotate{2});
    REQUIRE(focus.focused().oscillator == 0);
    REQUIRE(focus.focused().property == Property::Attack);

    control.handle(Click{});

    REQUIRE(focus.edited());
  }

  SUBCASE("should change edited parameter on rotate event") {
    Control control{model, focus};
    control.handle(Rotate{7});
    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Volume);
    control.handle(Click{});
    REQUIRE(focus.edited());

    model.channels.at(1).volume = 12345;
    control.handle(Rotate{-2});

    REQUIRE(model.channels.at(1).volume != 12345);
  }

  SUBCASE("should confirm edited parameter on click event") {
    Control control{model, focus};

    control.handle(Rotate{10});
    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Sustain);
    control.handle(Click{});
    control.handle(Rotate{1});

    control.handle(Click{});

    REQUIRE(not focus.edited());
  }
}

TEST_CASE("control") {
  Model model{};
  Focus focus{};

  SUBCASE("should change volume on rotate event") {
    Control control{model, focus};
    model.channels.at(1).volume = 1000;

    control.handle(Rotate{7});
    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Volume);
    control.handle(Click{});
    REQUIRE(focus.edited());

    control.handle(Rotate{-2});

    REQUIRE(model.channels.at(1).volume == 998);
  }

  SUBCASE("should not change other parameters") {
    Control control{model, focus};
    model.channels.at(0).volume = 1000;
    model.channels.at(1).volume = 1111;
    model.channels.at(2).volume = 2000;

    model.channels.at(1).attack = 3000;
    model.channels.at(1).release = 4000;

    model.channels.at(1).wave = WaveForm::Sawtooth;

    control.handle(Rotate{7});
    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Volume);
    control.handle(Click{});
    REQUIRE(focus.edited());

    control.handle(Rotate{-2});
    REQUIRE(model.channels.at(1).volume == 1109);

    REQUIRE(model.channels.at(0).volume == 1000);
    REQUIRE(model.channels.at(2).volume == 2000);

    REQUIRE(model.channels.at(1).attack == 3000);
    REQUIRE(model.channels.at(1).release == 4000);

    REQUIRE(model.channels.at(1).wave == WaveForm::Sawtooth);
  }

  SUBCASE("should change volume on rotate event") {
    Control control{model, focus};

    control.handle(Rotate{9});
    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Decay);
    control.handle(Click{});
    REQUIRE(focus.edited());

    std::vector<int> steps{-2, -1, 0, 1, 2};
    for (auto step : steps) {
      model.channels.at(1).decay = 30000;
      control.handle(Rotate{step});

      REQUIRE(model.channels.at(1).decay == 30000 + step);
    }
  }

  SUBCASE("shold change parameter value exponentially on fast rotation") {
    Control control{model, focus};

    control.handle(Rotate{9});
    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Decay);
    control.handle(Click{});
    REQUIRE(focus.edited());

    struct Data {
      int steps;
      uint16_t value;
    };

    std::vector<Data> data{{3, 30000 + 64},   {4, 30000 + 256},
                           {6, 30000 + 4096}, {7, 30000 + 16384},
                           {-3, 30000 - 64},  {-5, 30000 - 1024},
                           {-6, 30000 - 4096}};

    for (auto item : data) {
      model.channels.at(1).decay = 30000;
      control.handle(Rotate{item.steps});

      REQUIRE(model.channels.at(1).decay == item.value);
    }
  }

  SUBCASE("should not overflow parameter value") {
    Control control{model, focus};

    control.handle(Rotate{9});
    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Decay);
    control.handle(Click{});
    REQUIRE(focus.edited());

    model.channels.at(1).decay = 65535;
    control.handle(Rotate{1});
    REQUIRE(model.channels.at(1).decay == 65535);

    model.channels.at(1).decay = 30000;
    control.handle(Rotate{9});
    REQUIRE(model.channels.at(1).decay == 65535);
  }

  SUBCASE("should not underflow parameter value") {
    Control control{model, focus};

    control.handle(Rotate{9});
    REQUIRE(focus.focused().oscillator == 1);
    REQUIRE(focus.focused().property == Property::Decay);
    control.handle(Click{});
    REQUIRE(focus.edited());

    model.channels.at(1).decay = 1;
    control.handle(Rotate{-2});
    REQUIRE(model.channels.at(1).decay == 0);

    model.channels.at(1).decay = 30000;
    control.handle(Rotate{-8});
    REQUIRE(model.channels.at(1).decay == 0);
  }
}

TEST_CASE("notification") {
  Model model{};
  Focus focus{};

  SUBCASE("should notify when parameter is focused") {
    std::vector<ControlEvent> spy;

    Control control{model, focus};
    control.onEvent([&spy](ControlEvent event) { spy.push_back(event); });

    control.handle(Rotate{-3});

    REQUIRE(spy.size() == 1);
    REQUIRE(spy.back() == ControlEvent::Focus);
  }

  SUBCASE("should notify when parameter is edited") {
    std::vector<ControlEvent> spy;

    Control control{model, focus};
    control.onEvent([&spy](ControlEvent event) { spy.push_back(event); });

    control.handle(Rotate{2});
    control.handle(Click{});

    REQUIRE(spy.size() == 2);
    REQUIRE(spy.back() == ControlEvent::Edit);
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
    REQUIRE(spy.back() == ControlEvent::Change);
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
    REQUIRE(spy.back() == ControlEvent::Confirm);
  }
}

} // namespace
