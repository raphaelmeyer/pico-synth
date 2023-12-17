#pragma once

#include "synth_config.h"

struct Message;

class Synth {
public:
  Synth(config::Synth const &config) : config_{config} {};

  void init();
  void handle(uint32_t packet);

private:
  void play(uint8_t channel, uint8_t note);
  void release(uint8_t channel);

  void send(Message const &message);

  config::Synth const config_;
};
