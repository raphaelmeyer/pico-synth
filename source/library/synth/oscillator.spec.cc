#include "oscillator.h"

#include <spec.h>

using namespace spec;

namespace {

describe decoding{"Oscillator", {it{"should oscillate", [] {
                                      Oscillator oscillator{{}};
                                      check(oscillator.next_value() == 0);
                                    }}}};

} // namespace
