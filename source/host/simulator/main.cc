#include <synth/device/random.h>
#include <synth/synth/envelope-generator.h>
#include <synth/synth/oscillator.h>

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

  Oscillator square{48000, random};
  square.set_type(WaveForm::Square);

  EnvelopeGenerator eg{48000, square};

  eg.set_volume(32784);
  eg.set_attack(100);
  eg.set_decay(50);
  eg.set_sustain(40000);
  eg.set_release(150);

  // c4
  square.set_frequency(1046);

  Oscillator triangle{48000, random};
  triangle.set_type(WaveForm::Triangle);
  triangle.set_frequency(1046);

  Oscillator sawtooth{48000, random};
  sawtooth.set_type(WaveForm::Sawtooth);
  sawtooth.set_frequency(1046);

  Oscillator noise{48000, random};
  noise.set_type(WaveForm::Noise);
  noise.set_frequency(1046);

  std::cout << "SquareEnv"
            << " , "
            << "Triangle"
            << " , "
            << "Sawtooth"
            << " , "
            << "Noise"
            << "\n";

  eg.trigger();
  for (int i = 0; i < 15'000; ++i) {
    std::cout << eg.next_value() << " , " << triangle.next_value() << " , "
              << sawtooth.next_value() << " , " << noise.next_value() << "\n";
  }
  eg.release();
  for (int i = 0; i < 10'000; ++i) {
    std::cout << eg.next_value() << " , " << triangle.next_value() << " , "
              << sawtooth.next_value() << " , " << noise.next_value() << "\n";
  }
}
