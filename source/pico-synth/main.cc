
#include "config.h"
#include "i2s.h"
#include "oscillator.h"

#include <hardware/pio.h>
#include <pico/time.h>
#include <pico/types.h>

namespace {

Config const config{.audio = {.sampling_frequency = 48000},
                    .i2s = {.data = 12, .clock_ws_base = 10, .pio = pio0}};

} // namespace

namespace {

Oscillator left{config.audio, 440};
Oscillator right{config.audio, 680};
I2S i2s{config.i2s, config.audio};

} // namespace

int main() {

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, true);
  gpio_put(PICO_DEFAULT_LED_PIN, true);

  i2s.init();

  for (;;) {
    i2s.output_sample(left.next_value(), right.next_value());
  }
}
