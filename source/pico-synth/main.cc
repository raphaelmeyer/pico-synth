
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
Oscillator oscillator2{config.audio};

EnvelopeGenerator eg{config.audio, oscillator};
EnvelopeGenerator eg2{config.audio, oscillator2};

I2S i2s{config.i2s, config.audio.sampling_frequency};

uint16_t P = 0;

uint16_t C2 = 65;
uint16_t D2 = 73;
uint16_t E2 = 82;
uint16_t F2 = 87;
uint16_t G2 = 98;
uint16_t Gs2 = 104;
uint16_t A2 = 110;
uint16_t B2 = 123;

uint16_t C3 = 131;
uint16_t D3 = 147;
uint16_t E3 = 165;
uint16_t Gs3 = 208;
uint16_t A3 = 220;
uint16_t B3 = 247;

uint16_t C4 = 262;
uint16_t D4 = 294;
uint16_t E4 = 330;
uint16_t Gs4 = 415;
uint16_t A4 = 440;
uint16_t B4 = 494;

uint16_t C5 = 523;
uint16_t D5 = 587;
uint16_t E5 = 659;
uint16_t F5 = 698;
uint16_t G5 = 784;
uint16_t A5 = 880;

} // namespace

int main() {

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, true);

  i2s.init();

  oscillator.set_type(Form::Square);
  oscillator2.set_type(Form::Triangle);

  eg.set_attack(100);
  eg.set_decay(50);
  eg.set_sustain(2048);
  eg.set_release(400);

  eg2.set_attack(200);
  eg2.set_decay(50);
  eg2.set_sustain(4096);
  eg2.set_release(200);

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

  std::vector<Note> bass{
      {E2, 2},  {E3, 2},  {E2, 2},  {E3, 2},  {E2, 2},
      {E3, 2},  {E2, 2},  {E3, 2}, //

      {A2, 2},  {A3, 2},  {A2, 2},  {A3, 2},  {A2, 2},
      {A3, 2},  {A2, 2},  {A3, 2}, //

      {Gs2, 2}, {Gs3, 2}, {Gs2, 2}, {Gs3, 2}, {E2, 2},
      {E3, 2},  {E2, 2},  {E3, 2}, //

      {A2, 2},  {A3, 2},  {A2, 2},  {A3, 2},  {A2, 2},
      {A3, 2},  {B2, 2},  {C3, 2}, //

      {D3, 2},  {D2, 2},  {P, 2},   {D2, 2},  {P, 2},
      {D2, 2},  {A2, 2},  {F2, 2}, //

      {C2, 2},  {C3, 2},  {P, 2},   {C3, 2},  {C2, 2},
      {G2, 2},  {G2, 2},  {P, 2}, //

      {B2, 2},  {B3, 2},  {P, 2},   {B3, 2},  {P, 2},
      {E3, 2},  {P, 2},   {Gs3, 2}, //

      {A2, 2},  {E3, 2},  {A2, 2},  {E3, 2},  {A2, 8}, //

      {A3, 2},  {E4, 2},  {A3, 2},  {E4, 2},  {A3, 2},
      {E4, 2},  {A3, 2},  {E4, 2}, //

      {Gs3, 2}, {E4, 2},  {Gs3, 2}, {E4, 2},  {Gs3, 2},
      {E4, 2},  {Gs3, 2}, {E4, 2}, //

      {A3, 2},  {E4, 2},  {A3, 2},  {E4, 2},  {A3, 2},
      {E4, 2},  {A3, 2},  {E4, 2}, //

      {Gs3, 2}, {E4, 2},  {Gs3, 2}, {E4, 2},  {Gs3, 2},
      {E4, 2},  {Gs3, 2}, {E4, 2}, //

      {A3, 2},  {E4, 2},  {A3, 2},  {E4, 2},  {A3, 2},
      {E4, 2},  {A3, 2},  {E4, 2}, //

      {Gs3, 2}, {E4, 2},  {Gs3, 2}, {E4, 2},  {Gs3, 2},
      {E4, 2},  {Gs3, 2}, {E4, 2}, //

      {A3, 2},  {E4, 2},  {A3, 2},  {E4, 2},  {A3, 2},
      {E4, 2},  {A3, 2},  {E4, 2}, //

      {Gs3, 2}, {E4, 2},  {Gs3, 2}, {E4, 2},  {P, 8} //
  };

  int count = 0;
  int rem = 0;
  int rem2 = 0;

  int beat = 0;

  auto it = melody.cbegin();
  auto jt = bass.cbegin();

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

      if (rem2 == 0) {
        if (jt->note == P) {
          eg2.release();
        } else {
          oscillator2.set_frequency(jt->note);
          eg2.trigger();
        }

        rem2 = jt->length;

        ++jt;
        if (jt == bass.cend()) {
          jt = bass.cbegin();
        }
      }
      --rem2;
    }

    ++count;
    if (count >= 6000) {
      count = 0;
    }

    auto const left = eg.next_value();
    auto const right = eg2.next_value();
    i2s.output_sample(left, right);
  }
}
