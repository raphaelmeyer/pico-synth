#include "channel.h"
#include "config.h"
#include "control.h"
#include "mixer.h"

#include <i2s.h>
#include <message.h>
#include <pico-random.h>

#include <hardware/gpio.h>
#include <hardware/pio.h>
#include <hardware/spi.h>
#include <pico/binary_info.h>
#include <pico/multicore.h>

namespace {

Config const hw_config{
    .synth = {.sampling_rate = 48000},
    .i2s = {.data = 12, .clock_ws_base = 10, .pio = pio0},
    .control{
        .mosi = 16, .miso = 19, .clock = 18, .chip_select = 17, .spi = spi0}};

PicoRandom random{};
I2S i2s{hw_config.i2s, hw_config.synth.sampling_rate};

Channels channels{{{hw_config.synth, random},
                   {hw_config.synth, random},
                   {hw_config.synth, random},
                   {hw_config.synth, random}}};

Control control{hw_config.control, channels};
Mixer mixer{channels, i2s};

void main_core1() {
  i2s.init();
  multicore_fifo_push_blocking(1234);
  for (;;) {
    mixer.mix();
  }
}

} // namespace

int main() {
  bi_decl(bi_3pins_with_names(10, "BCLK", 11, "WSEL", 12, "DIN"));
  bi_decl(bi_2pins_with_func(16, 17, GPIO_FUNC_SPI));
  bi_decl(bi_2pins_with_func(18, 19, GPIO_FUNC_SPI));

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, true);
  gpio_put(PICO_DEFAULT_LED_PIN, false);

  control.init();

  multicore_launch_core1(main_core1);
  multicore_fifo_pop_blocking();

  gpio_put(PICO_DEFAULT_LED_PIN, true);

  for (;;) {
    auto const message = control.read();
    control.dispatch(message);
  }
}
