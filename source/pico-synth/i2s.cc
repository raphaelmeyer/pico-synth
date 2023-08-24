#include "i2s.h"

#include "i2s.pio.h"

#include <hardware/clocks.h>

void I2S::init() {
  auto const offset = pio_add_program(config_.pio, &i2s_program);
  sm_ = pio_claim_unused_sm(config_.pio, true);

  auto sm_config = i2s_program_get_default_config(offset);

  sm_config_set_out_pins(&sm_config, config_.data, 1);
  sm_config_set_sideset_pins(&sm_config, config_.clock_ws_base);

  pio_sm_set_consecutive_pindirs(config_.pio, sm_, config_.clock_ws_base, 2,
                                 true);
  pio_sm_set_consecutive_pindirs(config_.pio, sm_, config_.data, 1, true);

  pio_gpio_init(config_.pio, config_.clock_ws_base);
  pio_gpio_init(config_.pio, config_.clock_ws_base + 1);
  pio_gpio_init(config_.pio, config_.data);

  sm_config_set_out_shift(&sm_config, false, true, 32);
  sm_config_set_fifo_join(&sm_config, PIO_FIFO_JOIN_TX);

  int const cycles_per_sample = 2 * 32;
  auto const div =
      clock_get_hz(clk_sys) / (audio_.sampling_frequency * cycles_per_sample);
  sm_config_set_clkdiv_int_frac(&sm_config, div, 0);

  pio_sm_init(config_.pio, sm_, offset, &sm_config);
  pio_sm_set_enabled(config_.pio, sm_, true);
}

void I2S::output_sample(uint16_t left, uint16_t right) {
  uint32_t const value = (left << 16) | right;
  pio_sm_put_blocking(config_.pio, sm_, value);
}
