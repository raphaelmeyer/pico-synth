#include <ApprovalTests/Approvals.h>
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

TEST_CASE("Triggering a note produces sound") {
  FakeRandom random{};
  Synth synth{48'000, random};

  synth.handle(Message{.address = 0, .command = SetVolume{.volume = 42000}});
  synth.handle(
      Message{.address = 0, .command = SetFrequency{.frequency = 1760}});
  synth.handle(Message{.address = 0,
                       .command = SetWaveForm{.wave = WaveForm::Sawtooth}});
  synth.handle(Message{.address = 0, .command = SetAttack{20}}); // 960 samples
  synth.handle(Message{.address = 0, .command = SetDecay{15}});  // 720 samples
  synth.handle(Message{.address = 0,
                       .command = SetSustain{55555}}); // 100 ms -> 4800 samples
  synth.handle(
      Message{.address = 0, .command = SetRelease{25}}); // 1200 samples

  std::vector<Sample> samples{7700, Sample{}};

  std::ranges::generate_n(samples.begin(), 10,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{.address = 0, .command = Trigger{}});

  std::ranges::generate_n(samples.begin() + 10, 6480,
                          [&synth] { return synth.next_sample(); });

  synth.handle(Message{.address = 0, .command = Release{}});

  std::ranges::generate(samples.begin() + 10 + 6480, samples.end(),
                        [&synth] { return synth.next_sample(); });

  ApprovalTests::Approvals::verifyAll(samples);
}

} // namespace
