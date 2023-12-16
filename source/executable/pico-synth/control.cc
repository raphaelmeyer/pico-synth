#include "control.h"

#include "channel.h"

#include <message.h>

#include <hardware/gpio.h>
#include <hardware/spi.h>

Control::Control(config::Control const &config, Channels &channels)
    : config_{config}, channels_{channels} {
  spi_init(config_.spi, 5'000'000);
  spi_set_slave(config_.spi, true);

  gpio_set_function(config_.mosi, GPIO_FUNC_SPI);
  gpio_set_function(config_.miso, GPIO_FUNC_SPI);
  gpio_set_function(config_.clock, GPIO_FUNC_SPI);
  gpio_set_function(config_.chip_select, GPIO_FUNC_SPI);
}

Message Control::read() {
  Word header{};
  Word data{};
  spi_read_blocking(config_.spi, 0, header.data(), 2);
  if (Message::has_data(header)) {
    spi_read_blocking(config_.spi, 0, data.data(), 2);
  }
  return Message::decode(header, data);
}

void Control::dispatch(Message const &message) {
  auto const id = message.address & 0x0Fu;
  if (id >= channels_.size()) {
    return;
  }

  auto &channel = channels_.at(id);

  if (std::holds_alternative<Trigger>(message.command)) {
    channel.tone_generator.trigger();
  } else if (std::holds_alternative<Release>(message.command)) {
    channel.tone_generator.release();
  } else if (auto const *wr = std::get_if<WriteRegister>(&message.command);
             wr != nullptr) {
    switch (wr->reg) {
    case Register::Control:
      break;
    case Register::Frequency:
      channel.tone_generator.set_frequency(wr->data);
      break;
    case Register::Attack:
      channel.tone_generator.set_attack(wr->data);
      break;
    case Register::Decay:
      channel.tone_generator.set_decay(wr->data);
      break;
    case Register::Sustain:
      channel.tone_generator.set_sustain(wr->data);
      break;
    case Register::Release:
      channel.tone_generator.set_release(wr->data);
      break;
    }
  }
}
