#include "config.h"
#include "control.h"
#include "tone-generator.h"

#include <envelope-generator.h>
#include <i2s.h>
#include <message.h>
#include <oscillator.h>
#include <pico-random.h>

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
    .control{
        .mosi = 16, .miso = 19, .clock = 18, .chip_select = 17, .spi = spi0}};

PicoRandom random{};

struct Instrument {
  Instrument()
      : oscillator{config.synth, random}, envelope_generator{config.synth,
                                                             oscillator},
        tone_generator{oscillator, envelope_generator} {}

  Oscillator oscillator;
  EnvelopeGenerator envelope_generator;
  ToneGenerator tone_generator;
};

std::array<Instrument, 4> instruments{};

I2S i2s{config.i2s, config.synth.sampling_rate};

Control control{config.control};

void second_core_entry() {
  i2s.init();
  multicore_fifo_push_blocking(1234);
  for (;;) {
    auto const left = instruments.at(0).tone_generator.next_value() +
                      instruments.at(1).tone_generator.next_value();
    auto const right = instruments.at(2).tone_generator.next_value() +
                       instruments.at(3).tone_generator.next_value();
    i2s.output_sample(left, right);
  }
}

void dispatch(Message const &message) {
  auto const channel = message.address & 0x0F;
  if (channel >= 4) {
    return;
  }

  auto &tone_generator = instruments.at(channel).tone_generator;

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
  bi_decl(bi_2pins_with_func(16, 17, GPIO_FUNC_SPI));
  bi_decl(bi_2pins_with_func(18, 19, GPIO_FUNC_SPI));

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, true);

  multicore_launch_core1(second_core_entry);
  multicore_fifo_pop_blocking();

  gpio_put(PICO_DEFAULT_LED_PIN, true);

  for (;;) {
    auto const message = control.read();
    dispatch(message);
  }
}
