
#include <pico/stdio_usb.h>
#include <pico/stdlib.h>
#include <pico/time.h>

#include <functional>

namespace runner {

void verify(std::function<void()> run_verify) {
  stdio_init_all();

  while (not stdio_usb_connected()) {
    sleep_ms(100);
  }

  run_verify();

  stdio_flush();

  // Do not exit to stay accessible for picotool
  for (;;) {
    sleep_ms(100);
  }
}

} // namespace runner
