#include <doctest/doctest.h>

#include <synth/control/focus.h>

namespace {

TEST_CASE("should select wave form of first oscillator by default") {
  Focus focus{};

  REQUIRE(focus.focused().oscillator == 0);
  REQUIRE(focus.focused().property == Property::WaveForm);
}

TEST_CASE("should focus next parameter") {
  Focus focus{};

  focus.scroll(1);
  REQUIRE(focus.focused().oscillator == 0);
  REQUIRE(focus.focused().property == Property::Volume);
}

TEST_CASE("should focus previous parameter") {
  Focus focus{};

  focus.scroll(-1);
  REQUIRE(focus.focused().oscillator == 3);
  REQUIRE(focus.focused().property == Property::Release);
}

TEST_CASE("should focus parameter by defined order when scrolling") {
  Focus focus{};

  focus.scroll(2);
  REQUIRE(focus.focused().oscillator == 0);
  REQUIRE(focus.focused().property == Property::Attack);

  focus.scroll(-4);
  REQUIRE(focus.focused().oscillator == 3);
  REQUIRE(focus.focused().property == Property::Sustain);

  focus.scroll(13);
  REQUIRE(focus.focused().oscillator == 1);
  REQUIRE(focus.focused().property == Property::Release);

  focus.scroll(4);
  REQUIRE(focus.focused().oscillator == 2);
  REQUIRE(focus.focused().property == Property::Decay);
}

TEST_CASE("should cycle focus when scrolling") {
  Focus focus{};

  focus.scroll(25);
  REQUIRE(focus.focused().oscillator == 0);
  REQUIRE(focus.focused().property == Property::Volume);

  focus.scroll(-26);
  REQUIRE(focus.focused().oscillator == 3);
  REQUIRE(focus.focused().property == Property::Release);
}

TEST_CASE("should not be editing by default") {
  Focus focus{};

  REQUIRE(not focus.edited());
}

TEST_CASE("should allow to edit focused parameter") {
  Focus focus{};
  focus.scroll(7);

  focus.edit();
  REQUIRE(focus.edited());
}

TEST_CASE("Should stop editing parameter after confirm") {
  Focus focus{};
  focus.scroll(7);

  focus.edit();
  focus.confirm();
  REQUIRE(not focus.edited());
}

TEST_CASE("should abort editing when scroll away") {
  Focus focus{};
  focus.scroll(7);
  focus.edit();

  focus.scroll(1);
  REQUIRE(not focus.edited());
}

} // namespace
