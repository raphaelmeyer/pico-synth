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

describe oscillator{"Oscillator",
                    {it{"should oscillate", [] {
                          Oscillator oscillator{{.sampling_rate = 48000}, rnd};
                          oscillator.set_frequency(440);
                          check(oscillator.next_value() == 0);
                        }}}};

} // namespace
