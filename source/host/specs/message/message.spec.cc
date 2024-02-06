#include <doctest/doctest.h>

#include <synth/message/message.h>
#include <variant>

TEST_CASE("should encode and decode a trigger message") {
  Message trigger{.address = 0x1, .command = Trigger{}};

  auto const encoded = encode(trigger);
  auto const decoded = decode(encoded);

  REQUIRE(decoded.address == 0x1);
  REQUIRE(std::holds_alternative<Trigger>(decoded.command));
}

TEST_CASE("should encode and decode a release message") {
  Message release{.address = 0x3, .command = Release{}};

  auto const encoded = encode(release);
  auto const decoded = decode(encoded);

  REQUIRE(decoded.address == 0x3);
  REQUIRE(std::holds_alternative<Release>(decoded.command));
}

TEST_CASE("a release message should not have any data") {}

TEST_CASE("should encode and decode a write register message") {
  Message set_volume{.address = 0x2,
                     .command =
                         SetRegister{.reg = Register::Volume, .value = 0xabcd}};

  auto const encoded = encode(set_volume);
  auto const decoded = decode(encoded);
  auto const command = std::get_if<SetRegister>(&decoded.command);

  REQUIRE(decoded.address == 0x2);
  REQUIRE(command != nullptr);
  REQUIRE(command->reg == Register::Volume);
}
