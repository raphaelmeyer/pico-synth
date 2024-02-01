#include "synth.h"
#include "message.h"

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

  Message set_frequency{
      .address = channel,
      .command = WriteRegister{.reg = Register::Frequency, .data = frequency}};
  send(set_frequency);

  Message trigger{.address = channel, .command = Trigger{}};
  send(trigger);
}

void Synth::release(uint8_t channel) {
  Message release{.address = channel, .command = Release{}};
  send(release);
}

void Synth::send(Message const &message) {
  const auto header = message.encode();
  const auto data = message.data();

  spi_write_blocking(config_.spi, header.data(), header.size());
  if (data.has_value()) {
    spi_write_blocking(config_.spi, data.value().data(), data.value().size());
  }
}
