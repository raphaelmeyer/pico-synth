#include "tone-generator.h"

#include <envelope-generator.h>
#include <oscillator.h>

#include <pico/time.h>

ToneGenerator::ToneGenerator(Oscillator &oscillator,
                             EnvelopeGenerator &envelope_generator)
    : oscillator_(oscillator), envelope_generator_(envelope_generator) {
  mutex_init(&m_);
}

void ToneGenerator::trigger() {
  while (!mutex_try_enter(&m_, nullptr)) {
    sleep_us(10);
  }
  envelope_generator_.trigger();
  mutex_exit(&m_);
}

void ToneGenerator::release() {
  while (!mutex_try_enter(&m_, nullptr)) {
    sleep_us(10);
  }
  envelope_generator_.release();
  mutex_exit(&m_);
}

void ToneGenerator::set_frequency(uint16_t frequency) {
  while (!mutex_try_enter(&m_, nullptr)) {
    sleep_us(10);
  }
  oscillator_.set_frequency(frequency);
  mutex_exit(&m_);
}

void ToneGenerator::set_attack(uint16_t attack) {
  while (!mutex_try_enter(&m_, nullptr)) {
    sleep_us(10);
  }
  envelope_generator_.set_attack(attack);
  mutex_exit(&m_);
}

void ToneGenerator::set_decay(uint16_t decay) {
  while (!mutex_try_enter(&m_, nullptr)) {
    sleep_us(10);
  }
  envelope_generator_.set_decay(decay);
  mutex_exit(&m_);
}

void ToneGenerator::set_sustain(uint16_t sustain) {
  while (!mutex_try_enter(&m_, nullptr)) {
    sleep_us(10);
  }
  envelope_generator_.set_sustain(sustain);
  mutex_exit(&m_);
}

void ToneGenerator::set_release(uint16_t release) {
  while (!mutex_try_enter(&m_, nullptr)) {
    sleep_us(10);
  }
  envelope_generator_.set_release(release);
  mutex_exit(&m_);
}

uint16_t ToneGenerator::next_value() {
  mutex_enter_blocking(&m_);
  uint16_t const value = envelope_generator_.next_value();
  mutex_exit(&m_);

  return value;
}
