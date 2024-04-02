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

TEST_CASE("ADSR parameters") {
  FakeRandom random{};
  Synth synth{48'000, random};

  uint16_t attack{42};
  uint16_t decay{64};
  uint16_t sustain{43210};
  uint16_t release{123};

  SUBCASE("no attack") { attack = 0; }

  SUBCASE("no decay") { decay = 0; }

  SUBCASE("no sustain") { sustain = 0; }

  SUBCASE("no release") { release = 0; }

  synth.handle(Message{3, SetVolume{34567}});
  synth.handle(Message{3, SetFrequency{2349}});
  synth.handle(Message{3, SetWaveForm{WaveForm::Square}});

  synth.handle(Message{3, SetAttack{attack}});
  synth.handle(Message{3, SetDecay{decay}});
  synth.handle(Message{3, SetSustain{sustain}});
  synth.handle(Message{3, SetRelease{release}});

  std::vector<Sample> samples{15000, Sample{}};
  std::ranges::generate_n(samples.begin(), 100,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{3, Trigger{}});

  std::ranges::generate_n(samples.begin() + 100, 7000,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{3, Release{}});

  std::ranges::generate(samples.begin() + 100 + 7000, samples.end(),
                        [&synth] { return synth.next_sample(); });

  auto const section = ApprovalTests::NamerFactory::appendToOutputFilename(
      std::to_string(attack) + "_" + std::to_string(decay) + "_" +
      std::to_string(sustain) + "_" + std::to_string(release));
  ApprovalTests::Approvals::verifyAll(samples);
}

TEST_CASE("All four oscillators") {
  FakeRandom random{};
  Synth synth{48'000, random};

  synth.handle(Message{0, SetVolume{8000}});
  synth.handle(Message{0, SetFrequency{880}});
  synth.handle(Message{0, SetWaveForm{WaveForm::Triangle}});
  synth.handle(Message{0, SetAttack{100}});
  synth.handle(Message{0, SetDecay{100}});
  synth.handle(Message{0, SetSustain{40000}});
  synth.handle(Message{0, SetRelease{100}});

  synth.handle(Message{1, SetVolume{8000}});
  synth.handle(Message{1, SetFrequency{1760}});
  synth.handle(Message{1, SetWaveForm{WaveForm::Square}});
  synth.handle(Message{1, SetAttack{50}});
  synth.handle(Message{1, SetDecay{150}});
  synth.handle(Message{1, SetSustain{40000}});
  synth.handle(Message{1, SetRelease{100}});

  synth.handle(Message{2, SetVolume{8000}});
  synth.handle(Message{2, SetFrequency{3520}});
  synth.handle(Message{2, SetWaveForm{WaveForm::Sawtooth}});
  synth.handle(Message{2, SetAttack{100}});
  synth.handle(Message{2, SetDecay{50}});
  synth.handle(Message{2, SetSustain{40000}});
  synth.handle(Message{2, SetRelease{150}});

  synth.handle(Message{3, SetVolume{8000}});
  synth.handle(Message{3, SetFrequency{7040}});
  synth.handle(Message{3, SetWaveForm{WaveForm::Square}});
  synth.handle(Message{3, SetAttack{150}});
  synth.handle(Message{3, SetDecay{100}});
  synth.handle(Message{3, SetSustain{40000}});
  synth.handle(Message{3, SetRelease{50}});

  std::vector<Sample> samples{20000, Sample{}};

  std::ranges::generate_n(samples.begin(), 10,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{0, Trigger{}});

  std::ranges::generate_n(samples.begin() + 10, 700,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{1, Trigger{}});

  std::ranges::generate_n(samples.begin() + 710, 700,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{2, Trigger{}});

  std::ranges::generate_n(samples.begin() + 1410, 700,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{3, Trigger{}});

  std::ranges::generate_n(samples.begin() + 2110, 7890,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{0, Release{}});

  std::ranges::generate_n(samples.begin() + 10000, 700,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{1, Release{}});

  std::ranges::generate_n(samples.begin() + 10700, 700,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{2, Release{}});

  std::ranges::generate_n(samples.begin() + 11400, 700,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{3, Release{}});

  std::ranges::generate(samples.begin() + 12100, samples.end(),
                        [&synth] { return synth.next_sample(); });

  ApprovalTests::Approvals::verifyAll(samples);
}

} // namespace
