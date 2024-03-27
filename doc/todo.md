# TODO

## Migration

- input device(s)
- add (and wire) control in midi adapter

## Overview

- Refactor project setup

  - Migrate and clean up implementation
  - Delete old implementation
  - Clean up (or remove) taskfile
  - Move content from README to doc, except for short hint how to get started

- midi: `powf` -> `<cmath>` vs `<pico/float.h>`

- Reset function for synth

  - Reset on {0xff, 0xff} independent of data or command? That would disable
    value 0xffff for all register
  - Define a reset command and just send it twice to handle case of spi out of
    sync?

- Smooth oscillator frequency change (contine with current level)
- Smooth trigger when not yet released

- Add tests running on target (verify calculations)
