
#include "config.h"
#include "envelope-generator.h"
#include "i2s.h"
#include "oscillator.h"

#include <hardware/pio.h>
#include <pico/time.h>
#include <pico/types.h>

#include <vector>

namespace {

Config const config{.audio = {.sampling_frequency = 48000},
                    .i2s = {.data = 12, .clock_ws_base = 10, .pio = pio0}};

} // namespace

namespace {

Oscillator oscillator{config.audio};

EnvelopeGenerator eg{config.audio, oscillator};

I2S i2s{config.i2s, config.audio};

uint16_t P = 0;

uint16_t Gs3 = 208;
uint16_t A3 = 220;
uint16_t B3 = 247;

uint16_t C4 = 262;
uint16_t D4 = 294;
uint16_t E4 = 330;
//
uint16_t Gs4 = 415;
uint16_t A4 = 440;
uint16_t B4 = 494;

uint16_t C5 = 523;
uint16_t D5 = 587;
uint16_t E5 = 659;
uint16_t F5 = 698;
uint16_t G5 = 784;
uint16_t A5 = 880;
// uint16_t B5 = 988;

// uint16_t C6 = 1047;
// uint16_t D6 = 1175;
// uint16_t E6 = 1319;

} // namespace

int main() {

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, true);

  i2s.init();

  oscillator.set_frequency(440);

  eg.set_attack(100);
  eg.set_decay(50);
  eg.set_sustain(4096);
  eg.set_release(400);

  struct Note {
    uint16_t note{};
    int32_t length{};
  };

  std::vector<Note> melody{
      {E5, 4}, {B4, 2}, {C5, 2},  {D5, 4}, {C5, 2},   {B4, 2}, //
      {A4, 4}, {A4, 2}, {C5, 2},  {E5, 4}, {D5, 2},   {C5, 2}, //
      {B4, 4}, {B4, 2}, {C5, 2},  {D5, 4}, {E5, 4},            //
      {C5, 4}, {A4, 4}, {A4, 4},  {P, 4},                      //

      {P, 2},  {D5, 4}, {F5, 2},  {A5, 4}, {G5, 2},   {F5, 2}, //
      {E5, 6}, {C5, 2}, {E5, 4},  {D5, 2}, {C5, 2},            //
      {B4, 4}, {B4, 2}, {C5, 2},  {D5, 4}, {E5, 4},            //
      {C5, 4}, {A4, 4}, {A4, 4},  {P, 4},                      //

      {E4, 8}, {C4, 8}, {D4, 8},  {B3, 8},                   //
      {C4, 8}, {A3, 8}, {Gs3, 8}, {B3, 8},                   //
      {E4, 8}, {C4, 8}, {D4, 8},  {B3, 8},                   //
      {C4, 4}, {E4, 4}, {A4, 4},  {A4, 4}, {Gs4, 12}, {P, 4} //
  };

  int count = 0;
  int rem = 0;

  int beat = 0;

  auto it = melody.cbegin();

  for (;;) {
    if (count == 0) {
      if (beat == 0) {
        gpio_put(PICO_DEFAULT_LED_PIN, true);
      }
      if (beat == 2) {
        gpio_put(PICO_DEFAULT_LED_PIN, false);
      }
      ++beat;
      if (beat >= 16) {
        beat = 0;
      }

      if (rem == 0) {
        if (it->note == P) {
          eg.release();
        } else {
          oscillator.set_frequency(it->note);
          eg.trigger();
        }
        rem = it->length;

        ++it;
        if (it == melody.cend()) {
          it = melody.cbegin();
        }
      }
      --rem;
    }

    ++count;
    if (count >= 6000) {
      count = 0;
    }

    auto const mono = eg.next_value();
    i2s.output_sample(mono, mono);
  }
}
