#pragma once

#include <pico/mutex.h>

class Oscillator;
class EnvelopeGenerator;

class ToneGenerator {
public:
  ToneGenerator(Oscillator &oscillator, EnvelopeGenerator &envelope_generator);

  void trigger();
  void release();

  void set_frequency(uint16_t frequency);

  void set_attack(uint16_t attack);
  void set_decay(uint16_t decay);
  void set_sustain(uint16_t sustain);
  void set_release(uint16_t release);

  uint16_t next_value();

private:
  mutex_t m_{};

  Oscillator &oscillator_;
  EnvelopeGenerator &envelope_generator_;
};
