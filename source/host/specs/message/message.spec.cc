#include <doctest/doctest.h>

#include <synth/message/message.h>

#include <deque>
#include <variant>

namespace {

Message send_and_receive(Message const &message) {
  std::deque<Word> channel{};

  send(message, [&](auto const &data) { channel.push_back(data); });

  return receive([&]() {
    auto const result = channel.front();
    channel.pop_front();
    return result;
  });
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
  Message set_volume{.address = 0x2,
                     .command =
                         SetRegister{.reg = Register::Volume, .value = 0xabcd}};

  auto const decoded = send_and_receive(set_volume);

  auto const command = std::get_if<SetRegister>(&decoded.command);

  REQUIRE(decoded.address == 0x2);
  REQUIRE(command != nullptr);
  REQUIRE(command->reg == Register::Volume);
  REQUIRE(command->value == 0xabcd);
}

TEST_CASE("send and receive a set frequency register message") {
  Message set_volume{
      .address = 0xa,
      .command = SetRegister{.reg = Register::Frequency, .value = 0x7654}};

  auto const decoded = send_and_receive(set_volume);

  auto const command = std::get_if<SetRegister>(&decoded.command);

  REQUIRE(decoded.address == 0xa);
  REQUIRE(command != nullptr);
  REQUIRE(command->reg == Register::Frequency);
  REQUIRE(command->value == 0x7654);
}
