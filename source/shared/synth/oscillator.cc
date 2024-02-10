#include "oscillator.h"
#include "wave-form.h"
#include "wave-generation.h"
#include <cstdint>

Oscillator::Oscillator(uint32_t sampling_rate) : counter_{sampling_rate} {}

uint16_t Oscillator::next_value() {
  const auto tick = counter_.next();
  return std::visit(
      [tick](WaveGeneration auto &wg) -> uint16_t { return wg.value(tick); },
      wave_generation_);
}

void Oscillator::set_type(WaveForm form) {
  switch (form) {
  case WaveForm::Sawtooth:
    wave_generation_ = Sawtooth{};
    break;

  default:
    wave_generation_ = None{};
    break;
  }
}

void Oscillator::set_frequency(uint16_t frequency) {
  counter_.set_frequency(frequency);
}
