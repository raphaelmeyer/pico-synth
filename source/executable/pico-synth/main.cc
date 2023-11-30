#include "config.h"
#include "control.h"
#include "tone-generator.h"

#include <envelope-generator.h>
#include <i2s.h>
#include <message.h>
#include <oscillator.h>

#include <hardware/gpio.h>
#include <hardware/pio.h>
#include <hardware/spi.h>
#include <pico/binary_info.h>
#include <pico/multicore.h>

#include <array>
#include <variant>
#include <vector>

namespace {

Config const config{
    .synth = {.sampling_rate = 48000},
    .i2s = {.data = 12, .clock_ws_base = 10, .pio = pio0},
    .control{.clock = 18, .mosi = 16, .chip_select = 17, .spi = spi0}};

Oscillator oscillator{config.synth};
EnvelopeGenerator envelope_generator{config.synth, oscillator};
ToneGenerator tone_generator{oscillator, envelope_generator};

I2S i2s{config.i2s, config.synth.sampling_rate};

Control control{config.control};

void second_core_entry() {
  for (;;) {
    auto const value = tone_generator.next_value();
    i2s.output_sample(value, value);
  }
}

void dispatch(Message const &message) {
  if (std::holds_alternative<Trigger>(message.command)) {
    tone_generator.trigger();
  } else if (std::holds_alternative<Release>(message.command)) {
    tone_generator.release();
  } else if (auto const *wr = std::get_if<WriteRegister>(&message.command);
             wr != nullptr) {
    switch (wr->reg) {
    case Register::Control:
      break;
    case Register::Frequency:
      tone_generator.set_frequency(wr->data);
      break;
    case Register::Attack:
      tone_generator.set_attack(wr->data);
      break;
    case Register::Decay:
      tone_generator.set_decay(wr->data);
      break;
    case Register::Sustain:
      tone_generator.set_sustain(wr->data);
      break;
    case Register::Release:
      tone_generator.set_release(wr->data);
      break;
    }
  }
}

} // namespace

int main() {

  bi_decl(bi_3pins_with_names(10, "BCLK", 11, "WSEL", 12, "DIN"));
  bi_decl(bi_3pins_with_func(16, 17, 18, GPIO_FUNC_SPI));

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, true);

  multicore_launch_core1(second_core_entry);

  gpio_put(PICO_DEFAULT_LED_PIN, true);

  for (;;) {
    auto const message = control.read();
    dispatch(message);
  }
}
