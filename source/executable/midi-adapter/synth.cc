#include "synth.h"

#include <array>

#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/float.h>

namespace {

namespace midi {

constexpr const uint8_t note_off = 0x80;
constexpr const uint8_t note_on = 0x90;

} // namespace midi

} // namespace

void Synth::init() {
  spi_init(config_.spi, 5'000'000);

  gpio_set_function(config_.mosi, GPIO_FUNC_SPI);
  gpio_set_function(config_.miso, GPIO_FUNC_SPI);
  gpio_set_function(config_.clock, GPIO_FUNC_SPI);
  gpio_set_function(config_.chip_select, GPIO_FUNC_SPI);
}

void Synth::handle(uint32_t packet) {
  auto const message = (packet >> 8) & 0xF0;
  auto const channel = (packet >> 8) & 0x0F;

  switch (message) {
  case midi::note_off:
    release(channel);
    break;

  case midi::note_on:
    play(channel, (packet >> 16) & 0x7F);
    break;

  default:
    break;
  }
}

void Synth::play(uint8_t channel, uint8_t note) {
  auto const frequency =
      static_cast<uint16_t>(powf(2, (note - 69) / 12.0) * 440);

  std::array<uint8_t, 2> message{};

  message[0] = channel;
  message[1] = 0x11;
  spi_write_blocking(config_.spi, message.data(), 2);

  message[0] = frequency & 0xFF;
  message[1] = frequency >> 8;
  spi_write_blocking(config_.spi, message.data(), 2);

  message[0] = channel;
  message[1] = 0x01;
  spi_write_blocking(config_.spi, message.data(), 2);
}

void Synth::release(uint8_t channel) {
  std::array<uint8_t, 2> message{};

  message[0] = channel;
  message[1] = 0x02;
  spi_write_blocking(config_.spi, message.data(), 2);
}
