#include "pico-random.h"

#include <pico/rand.h>

uint32_t PicoRandom::value() { return get_rand_32(); }
