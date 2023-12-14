#include "message.h"

#include <spec.h>

using namespace spec;

namespace {

describe encoding{
    "Encoding and decoding messages",
    {

        it{"should encode trigger message",
           [] {
             Message message{.command = Trigger{}};
             auto const encoded = message.encode();
             auto const decoded = Message::decode(encoded);

             check(std::holds_alternative<Trigger>(decoded.command));
           }},

        it{"should encode release message",
           [] {
             Message message{.command = Release{}};
             auto const encoded = message.encode();
             auto const decoded = Message::decode(encoded);

             check(std::holds_alternative<Release>(decoded.command));
           }}

    }};

} // namespace
