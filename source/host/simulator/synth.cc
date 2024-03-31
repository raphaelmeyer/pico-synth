#include "synth/message/message.h"
#include <synth/device/random.h>
#include <synth/synth/synth.h>

#include <iostream>
#include <random>

class CSLRandom : public Random {
public:
  CSLRandom() : gen_{rd_()} {};

  uint32_t value() override { return gen_(); }

private:
  std::random_device rd_{};
  std::mt19937 gen_;
};

int main() {
  CSLRandom random{};

  Synth synth{48'000, random};

  synth.handle(Message{0, SetWaveForm{WaveForm::Square}});
  synth.handle(Message{0, SetFrequency{1760}});
  synth.handle(Message{0, SetVolume{40000}});
  synth.handle(Message{0, SetAttack{80}});
  synth.handle(Message{0, SetDecay{20}});
  synth.handle(Message{0, SetSustain{32'784}});
  synth.handle(Message{0, SetRelease{200}});

  std::cout << "Left"
            << " , "
            << "Right"
            << "\n";

  synth.handle(Message{0, Trigger{}});
  for (int i = 0; i < 15'000; ++i) {
    auto const sample = synth.next_sample();
    std::cout << sample.left << " , " << sample.right << "\n";
  }
  synth.handle(Message{0, Release{}});
  for (int i = 0; i < 10'000; ++i) {
    auto const sample = synth.next_sample();
    std::cout << sample.left << " , " << sample.right << "\n";
  }
}
