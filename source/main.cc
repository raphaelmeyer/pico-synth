#include "i2s.pio.h"

#include <hardware/clocks.h>
#include <hardware/pio.h>
#include <pico/time.h>
#include <pico/types.h>

namespace {
constexpr uint const side_set_base_pin = 10;
constexpr uint const data_pin = 12;

constexpr uint const clock_pin = side_set_base_pin + 0;
constexpr uint const ws_pin = side_set_base_pin + 1;

constexpr uint const sampling_frequency = 48000;
} // namespace

class I2S {
public:
  I2S(pio_hw_t *const pio) : pio_{pio} {};

  void init() {
    auto const offset = pio_add_program(pio_, &i2s_program);
    sm_ = pio_claim_unused_sm(pio_, true);

    auto sm_config = i2s_program_get_default_config(offset);

    sm_config_set_out_pins(&sm_config, data_pin, 1);
    sm_config_set_sideset_pins(&sm_config, side_set_base_pin);

    pio_sm_set_consecutive_pindirs(pio_, sm_, side_set_base_pin, 2, true);
    pio_sm_set_consecutive_pindirs(pio_, sm_, data_pin, 1, true);

    pio_gpio_init(pio_, clock_pin);
    pio_gpio_init(pio_, ws_pin);
    pio_gpio_init(pio_, data_pin);

    sm_config_set_out_shift(&sm_config, false, true, 32);
    sm_config_set_fifo_join(&sm_config, PIO_FIFO_JOIN_TX);

    int const cycles_per_sample = 2 * 32;
    auto const div =
        clock_get_hz(clk_sys) / (sampling_frequency * cycles_per_sample);
    sm_config_set_clkdiv_int_frac(&sm_config, div, 0);

    pio_sm_init(pio_, sm_, offset, &sm_config);
    pio_sm_set_enabled(pio_, sm_, true);
  }

  void output_sample(uint16_t left, uint16_t right) {
    uint32_t const value = (left << 16) | right;
    pio_sm_put_blocking(pio_, sm_, value);
  }

private:
  pio_hw_t *const pio_{nullptr};
  int sm_{};
};

enum class Form { Square, Sawtooth, Sine };

class Oscillator {
public:
  Oscillator(uint16_t frequency) : frequency_{frequency} {
    offset_ = 65535ul * frequency_ / sampling_frequency;
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
I2S audio{pio0};

} // namespace

int main() {

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, true);
  gpio_put(PICO_DEFAULT_LED_PIN, true);

  audio.init();

  for (;;) {
    audio.output_sample(left.next_value(), right.next_value());
  }
}