#include "message.h"

#include <spec.h>

using namespace spec;

namespace {

describe decoding{"Decoding messages",
                  {it{"should decode a trigger message", [] {
                        auto const message = decode_message({0, 0, 0, 0});
                        check(std::holds_alternative<Trigger>(message.command));
                      }}}};

} // namespace
