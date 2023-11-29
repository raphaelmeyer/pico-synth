#include "config.h"
#include "tone-generator.h"

#include <envelope-generator.h>
#include <i2s.h>
#include <oscillator.h>

#include <hardware/gpio.h>
#include <hardware/pio.h>
#include <pico/binary_info.h>
#include <pico/multicore.h>

#include <vector>

namespace {

Config const config{.synth = {.sampling_rate = 48000},
                    .i2s = {.data = 12, .clock_ws_base = 10, .pio = pio0}};

Oscillator oscillator{config.synth};
EnvelopeGenerator envelope_generator{config.synth, oscillator};
ToneGenerator tone_generator{oscillator, envelope_generator};

I2S i2s{config.i2s, config.synth.sampling_rate};

void second_core_entry() {
  for (;;) {
    auto const value = tone_generator.next_value();
    i2s.output_sample(value, value);
  }
}

} // namespace

int main() {

  bi_decl(bi_3pins_with_names(14, "BCLK", 15, "WSEL", 16, "DIN"));

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, true);

  multicore_launch_core1(second_core_entry);

  gpio_put(PICO_DEFAULT_LED_PIN, true);

  for (;;) {
    // read message from spi
    // decode message
    // tone_generator.set_xxx(...)
  }
}
