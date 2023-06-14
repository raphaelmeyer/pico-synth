
#include "config.h"
#include "i2s.h"

#include <hardware/pio.h>
#include <pico/time.h>
#include <pico/types.h>

namespace {

Config const config{.audio = {.sampling_frequency = 48000},
                    .i2s = {.data = 12, .clock_ws_base = 10, .pio = pio0}};

} // namespace

enum class Form { Square, Sawtooth, Sine };

class Oscillator {
public:
  Oscillator(uint16_t frequency) : frequency_{frequency} {
    offset_ = 65535ul * frequency_ / config.audio.sampling_frequency;
  }

  uint16_t next_value() {
    value_ = (value_ + offset_) % 65536;
    return value_;
  }

  void set_type([[maybe_unused]] Form form) {}
  void set_frequency([[maybe_unused]] uint16_t frequency) {}

private:
  uint16_t value_ = 0;

  uint16_t frequency_{};
  uint16_t offset_{};
};

namespace {

Oscillator left{440};
Oscillator right{680};
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
