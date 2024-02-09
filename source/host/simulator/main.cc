#include "synth/synth/envelope-generator.h"
#include <iostream>

#include <synth/synth/oscillator.h>

int main() {

  Oscillator oscillator{48000};
  EnvelopeGenerator eg{48000, oscillator};

  eg.set_volume(32784);

  // c4
  oscillator.set_frequency(1046);

  eg.set_attack(100);
  eg.set_decay(50);
  eg.set_sustain(40000);
  eg.set_release(150);

  int tick = 0;

  eg.trigger();
  for (int i = 0; i < 15'000; ++i) {
    std::cout << tick << " , " << eg.next_value() << "\n";
    ++tick;
  }
  eg.release();
  for (int i = 0; i < 10'000; ++i) {
    std::cout << tick << " , " << eg.next_value() << "\n";
    ++tick;
  }
}
