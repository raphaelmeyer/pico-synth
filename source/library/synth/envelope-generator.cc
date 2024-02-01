#include "envelope-generator.h"

uint16_t EnvelopeGenerator::next_value() {

  if (state_ == State::Idle) {
    return 0;
  }

  uint32_t level = 0;

  if (state_ == State::Attack) {
    if (ticks_ > attack_) {
      ticks_ = 0;
      state_ = State::Decay;
    } else {
      level = (65535 * ticks_) / attack_;
    }
  }

  if (state_ == State::Decay) {
    if (ticks_ > decay_) {
      ticks_ = 0;
      state_ = State::Sustain;
    } else {
      level = 65535 - ((65535 - sustain_) * ticks_) / decay_;
    }
  }

  if (state_ == State::Sustain) {
    level = sustain_;
  }

  if (state_ == State::Release) {
    if (ticks_ > release_) {
      ticks_ = 0;
      state_ = State::Idle;
    } else {
      level = sustain_ - (sustain_ * ticks_) / release_;
    }
  }

  ++ticks_;

  auto const value = (source_.next_value() * level) / 65535;
  return (value * volume_) / 65535;
}

void EnvelopeGenerator::trigger() {
  state_ = State::Attack;
  ticks_ = 0;
}

void EnvelopeGenerator::release() {
  switch (state_) {
  case State::Attack:
  case State::Decay:
  case State::Sustain:
    state_ = State::Release;
    ticks_ = 0;

  case State::Idle:
  case State::Release:
    break;
  }
}

void EnvelopeGenerator::set_attack(uint16_t ms) {
  attack_ = ms * config_.sampling_rate / 1000;
}

void EnvelopeGenerator::set_decay(uint16_t ms) {
  decay_ = ms * config_.sampling_rate / 1000;
}

void EnvelopeGenerator::set_sustain(uint16_t level) { sustain_ = level; }

void EnvelopeGenerator::set_release(uint16_t ms) {
  release_ = ms * config_.sampling_rate / 1000;
}

void EnvelopeGenerator::set_volume(uint16_t volume) { volume_ = volume; }
