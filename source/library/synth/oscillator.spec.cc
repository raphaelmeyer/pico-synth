#include "oscillator.h"

#include <random.h>

#include <spec.h>

using namespace spec;

namespace {

class FakeRandom : public Random {
public:
  uint32_t value() override { return 7; }
};

FakeRandom rnd{};

describe decoding{"Oscillator", {it{"should oscillate", [] {
                                      Oscillator oscillator{{}, rnd};
                                      check(oscillator.next_value() == 0);
                                    }}}};

} // namespace
