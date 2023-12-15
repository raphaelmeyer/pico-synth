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

             auto const decoded = Message::decode(message.encode());

             expect(decoded.address).to_be(0x2A);
             check(std::holds_alternative<Trigger>(decoded.command));
           }},

        it{"should encode release message",
           [] {
             Message message{.address = 0xA5, .command = Release{}};

             auto const decoded = Message::decode(message.encode());

             expect(decoded.address).to_be(0xA5);
             check(std::holds_alternative<Release>(decoded.command));
           }},

        it{"should encode write register message",
           [] {
             Message message{.address = 0x17,
                             .command =
                                 WriteRegister{.reg = Register::Sustain}};

             auto const decoded = Message::decode(message.encode());
             expect(decoded.address).to_be(0x17);

             auto const cmd = std::get_if<WriteRegister>(&decoded.command);
             check(cmd != nullptr);
             check(cmd->reg == Register::Sustain);
           }},

        it{"should not return data for trigger message",
           [] {
             Message message{.address = 0x2A, .command = Trigger{}};

             check(not message.data());
           }},

        it{"should supply the encoded write register data",
           [] {
             Message message{.command = WriteRegister{.reg = Register::Attack,
                                                      .data = 0x6502}};

             check(message.data().has_value());
             auto const header = message.encode();

             check(Message::has_data(header));
           }},

        it{"should decode data from write register",
           [] {
             Message message{.address = 0x17,
                             .command = WriteRegister{
                                 .reg = Register::Frequency, .data = 0x1234}};
             auto const decoded =
                 Message::decode(message.encode(), message.data().value());

             expect(message.data().value().at(0)).to_be(0x34);
             expect(message.data().value().at(1)).to_be(0x12);

             auto const cmd = std::get_if<WriteRegister>(&decoded.command);
             check(cmd != nullptr);

             expect(cmd->data).to_be(0x1234);
           }}

    }};

} // namespace
