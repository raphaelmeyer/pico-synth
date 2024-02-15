#include "oscillator.h"

#include "wave-form.h"
#include "wave-generation.h"

Oscillator::Oscillator(uint32_t sampling_rate, Random &random)
    : random_{random}, counter_{sampling_rate} {}

uint16_t Oscillator::next_value() {
  const auto tick = counter_.next();
  return std::visit(
      [tick](WaveGeneration auto &wg) -> uint16_t { return wg.value(tick); },
      wave_generation_);
}

void Oscillator::set_type(WaveForm form) {
  switch (form) {
  case WaveForm::Noise:
    wave_generation_.emplace<Noise>(random_);
    break;

  case WaveForm::Sawtooth:
    wave_generation_.emplace<Sawtooth>();
    break;

  case WaveForm::Square:
    wave_generation_.emplace<Square>();
    break;

  case WaveForm::Triangle:
    wave_generation_.emplace<Triangle>();
    break;

  default:
    wave_generation_.emplace<None>();
    break;
  }
}

void Oscillator::set_frequency(uint16_t frequency) {
  counter_.set_frequency(frequency);
}
