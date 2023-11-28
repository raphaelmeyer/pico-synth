#include "demo.h"

#include <hardware/gpio.h>
#include <pico/binary_info.h>
#include <pico/multicore.h>

#include <vector>

namespace {
void second_core_entry() { run_demo(); }
} // namespace

int main() {

  bi_decl(bi_3pins_with_names(14, "BCLK", 15, "WSEL", 16, "DIN"));

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, true);

  multicore_launch_core1(second_core_entry);

  gpio_put(PICO_DEFAULT_LED_PIN, true);

  for (;;)
    ;
}
