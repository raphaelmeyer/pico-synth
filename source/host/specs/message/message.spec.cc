#include <doctest/doctest.h>

#include <synth/message/message.h>

#include <deque>
#include <variant>

namespace {

class Channel : public Sender, public Receiver {

public:
  void send(Word data) override { channel_.push_back(data); }

  Word receive() override {
    auto const result = channel_.front();
    channel_.pop_front();
    return result;
  }

private:
  std::deque<Word> channel_{};
};

Message send_and_receive(Message const &message) {
  Channel channel{};
  send_message(message, channel);
  return receive_message(channel);
}

} // namespace

TEST_CASE("send and receive a trigger message") {
  Message trigger{.address = 0x1, .command = Trigger{}};

  auto const decoded = send_and_receive(trigger);

  REQUIRE(decoded.address == 0x1);
  REQUIRE(std::holds_alternative<Trigger>(decoded.command));
}

TEST_CASE("send and receive a release message") {
  Message release{.address = 0x3, .command = Release{}};

  auto const decoded = send_and_receive(release);

  REQUIRE(decoded.address == 0x3);
  REQUIRE(std::holds_alternative<Release>(decoded.command));
}

TEST_CASE("send and receive a set volume register message") {
  Message set_volume{.address = 0x2, .command = SetVolume{.volume = 0xabcd}};

  auto const decoded = send_and_receive(set_volume);

  auto const command = std::get_if<SetVolume>(&decoded.command);

  REQUIRE(decoded.address == 0x2);
  REQUIRE(command != nullptr);
  REQUIRE(command->volume == 0xabcd);
}

TEST_CASE("send and receive a set frequency register message") {
  Message set_frequency{.address = 0xa,
                        .command = SetFrequency{.frequency = 0x7654}};

  auto const decoded = send_and_receive(set_frequency);

  auto const command = std::get_if<SetFrequency>(&decoded.command);

  REQUIRE(decoded.address == 0xa);
  REQUIRE(command != nullptr);
  REQUIRE(command->frequency == 0x7654);
}

TEST_CASE("send and receive a set wave form register message") {
  Message set_wave{.address = 0x7, .command = SetWaveForm{WaveForm::Triangle}};

  auto const decoded = send_and_receive(set_wave);

  auto const command = std::get_if<SetWaveForm>(&decoded.command);
  REQUIRE(decoded.address == 0x7);
  REQUIRE(command != nullptr);
  REQUIRE(command->wave == WaveForm::Triangle);
}
