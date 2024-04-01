#include <ApprovalTests.hpp>
#include <doctest/doctest.h>

#include <synth/device/random.h>
#include <synth/synth/synth.h>

#include <algorithm>

std::ostream &operator<<(std::ostream &os, const Sample &sample) {
  return os << sample.left << " " << sample.right;
}

namespace {

class FakeRandom : public Random {
public:
  uint32_t value() override { return 32000; }
};

TEST_CASE("All oscillators are silent by default") {
  FakeRandom random{};
  Synth synth{48'000, random};

  std::vector<Sample> samples{10000, Sample{}};
  std::ranges::generate(samples, [&synth] { return synth.next_sample(); });

  ApprovalTests::Approvals::verifyAll(samples);
}

TEST_CASE("ADSR envelope for single oscillator") {
  FakeRandom random{};
  Synth synth{48'000, random};

  uint8_t address{};
  uint16_t frequency{};
  WaveForm wave{};

  SUBCASE("Oscillator 0 with A4") {
    address = 0;
    frequency = 440.00 * 4;
    wave = WaveForm::Triangle;
  }

  SUBCASE("Oscillator 1 with D♯3/ E♭3") {
    address = 1;
    frequency = 155.56 * 4;
    wave = WaveForm::Square;
  }

  SUBCASE("Oscillator 2 with G7") {
    address = 2;
    frequency = 3135.96 * 4;
    wave = WaveForm::Sawtooth;
  }

  SUBCASE("Oscillator 3 with G♯6/F♭6") {
    address = 3;
    frequency = 1661.22 * 4;
    wave = WaveForm::Square;
  }

  synth.handle(Message{address, SetVolume{42000}});
  synth.handle(Message{address, SetFrequency{frequency}});
  synth.handle(Message{address, SetWaveForm{wave}});
  synth.handle(Message{address, SetAttack{20}});     // 960 samples
  synth.handle(Message{address, SetDecay{15}});      // 720 samples
  synth.handle(Message{address, SetSustain{55555}}); // 100 ms -> 4800 samples
  synth.handle(Message{address, SetRelease{25}});    // 1200 samples

  std::vector<Sample> samples{7700, Sample{}};

  std::ranges::generate_n(samples.begin(), 10,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{address, Trigger{}});

  std::ranges::generate_n(samples.begin() + 10, 6480,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{address, Release{}});

  std::ranges::generate(samples.begin() + 10 + 6480, samples.end(),
                        [&synth] { return synth.next_sample(); });

  auto const section = ApprovalTests::NamerFactory::appendToOutputFilename(
      "Oscillator_" + std::to_string(address));
  ApprovalTests::Approvals::verifyAll(samples);
}

} // namespace
