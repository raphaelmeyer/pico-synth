#include "message.h"

#include <spec.h>

using namespace spec;

namespace {

describe encoding{
    "Encoding and decoding messages",
    {

        it{"should encode trigger message",
           [] {
             Message message{.address = 0x2A, .command = Trigger{}};
             auto const encoded = message.encode();
             auto const decoded = Message::decode(encoded);

             expect(decoded.address).to_be(0x2A);
             check(std::holds_alternative<Trigger>(decoded.command));
           }},

        it{"should encode release message",
           [] {
             Message message{.address = 0xA5, .command = Release{}};
             auto const encoded = message.encode();
             auto const decoded = Message::decode(encoded);

             expect(decoded.address).to_be(0xA5);
             check(std::holds_alternative<Release>(decoded.command));
           }},

        it{"should encode write register message",
           [] {
             Message message{.address = 0x17,
                             .command =
                                 WriteRegister{.reg = Register::Sustain}};
             auto const encoded = message.encode();
             auto const decoded = Message::decode(encoded);

             expect(decoded.address).to_be(0x17);

             auto const cmd = std::get_if<WriteRegister>(&decoded.command);
             check(cmd != nullptr);
             check(cmd->reg == Register::Sustain);
           }}

    }};

} // namespace
