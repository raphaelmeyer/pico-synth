#include "control.h"

#include "channel.h"

#include <message.h>

#include <hardware/gpio.h>
#include <hardware/spi.h>

void Control::init() {
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
  uint8_t const id = message.address & 0x0F;
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

void Control::setup_demo_instruments() {
  channels_.at(0).oscillator.set_type(Form::Sawtooth);
  channels_.at(0).tone_generator.set_attack(50);
  channels_.at(0).tone_generator.set_decay(50);
  channels_.at(0).tone_generator.set_sustain(4096);
  channels_.at(0).tone_generator.set_release(50);

  channels_.at(1).oscillator.set_type(Form::Triangle);
  channels_.at(1).tone_generator.set_attack(150);
  channels_.at(1).tone_generator.set_decay(100);
  channels_.at(1).tone_generator.set_sustain(32768);
  channels_.at(1).tone_generator.set_release(200);

  channels_.at(2).oscillator.set_type(Form::Square);
  channels_.at(2).tone_generator.set_attack(50);
  channels_.at(2).tone_generator.set_decay(50);
  channels_.at(2).tone_generator.set_sustain(4096);
  channels_.at(2).tone_generator.set_release(50);

  channels_.at(3).oscillator.set_type(Form::Triangle);
  channels_.at(3).tone_generator.set_attack(10);
  channels_.at(3).tone_generator.set_decay(20);
  channels_.at(3).tone_generator.set_sustain(16384);
  channels_.at(3).tone_generator.set_release(50);
}
