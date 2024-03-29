#include <doctest/doctest.h>

#include <synth/control/control_event.h>
#include <synth/control/focus.h>
#include <synth/control/model.h>
#include <synth/control/synth_control.h>

#include "message_spy.h"
#include "synth/message/message.h"

namespace {

TEST_CASE("SynthControl") {
  MessageSpy spy{};
  Model model{};
  Focus focus{};
  SynthControl synth_control{model, focus, spy};

  SUBCASE("Change the wave form") {
    model.channels.at(1).wave = WaveForm::Sawtooth;
    focus.scroll(6);

    synth_control.handle(ControlEvent::Change);

    auto const messages = spy.decode();
    REQUIRE(messages.size() == 1);

    REQUIRE(messages.at(0).address == 1);
    auto const set_waveform = std::get_if<SetWaveForm>(&messages.at(0).command);
    REQUIRE(set_waveform != nullptr);
    REQUIRE(set_waveform->wave == WaveForm::Sawtooth);
  }

  SUBCASE("Change volume") {
    model.channels.at(2).volume = 12345;
    focus.scroll(13);

    synth_control.handle(ControlEvent::Change);

    auto const messages = spy.decode();
    REQUIRE(messages.size() == 1);

    REQUIRE(messages.at(0).address == 2);
    auto const set_volume = std::get_if<SetVolume>(&messages.at(0).command);
    REQUIRE(set_volume != nullptr);
    REQUIRE(set_volume->volume == 12345);
  }

  SUBCASE("Change attack") {
    model.channels.at(3).attack = 10101;
    focus.scroll(20);

    synth_control.handle(ControlEvent::Change);

    auto const messages = spy.decode();
    REQUIRE(messages.size() == 1);

    REQUIRE(messages.at(0).address == 3);
    auto const set_attack = std::get_if<SetAttack>(&messages.at(0).command);
    REQUIRE(set_attack != nullptr);
    REQUIRE(set_attack->attack == 10101);
  }

  SUBCASE("Change decay") {
    model.channels.at(0).decay = 20000;
    focus.scroll(3);

    synth_control.handle(ControlEvent::Change);

    auto const messages = spy.decode();
    REQUIRE(messages.size() == 1);

    REQUIRE(messages.at(0).address == 0);
    auto const set_decay = std::get_if<SetDecay>(&messages.at(0).command);
    REQUIRE(set_decay != nullptr);
    REQUIRE(set_decay->decay == 20000);
  }

  SUBCASE("Change sustain") {
    model.channels.at(1).sustain = 65432;
    focus.scroll(10);

    synth_control.handle(ControlEvent::Change);

    auto const messages = spy.decode();
    REQUIRE(messages.size() == 1);

    REQUIRE(messages.at(0).address == 1);
    auto const set_sustain = std::get_if<SetSustain>(&messages.at(0).command);
    REQUIRE(set_sustain != nullptr);
    REQUIRE(set_sustain->sustain == 65432);
  }

  SUBCASE("Change release") {
    model.channels.at(2).release = 42000;
    focus.scroll(17);

    synth_control.handle(ControlEvent::Change);

    auto const messages = spy.decode();
    REQUIRE(messages.size() == 1);

    REQUIRE(messages.at(0).address == 2);
    auto const set_release = std::get_if<SetRelease>(&messages.at(0).command);
    REQUIRE(set_release != nullptr);
    REQUIRE(set_release->release == 42000);
  }

  SUBCASE("Other events should be ignored") {
    for (auto event :
         {ControlEvent::Focus, ControlEvent::Edit, ControlEvent::Confirm}) {
      synth_control.handle(event);

      auto const messages = spy.decode();
      REQUIRE(messages.size() == 0);
    }
  }
}

} // namespace
