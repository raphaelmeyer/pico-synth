# TODO

## Migration

- Fix segv on uisim shutdown
- UI refresh values on screen change

## Overview

- Refactor project setup

  - Migrate and clean up implementation
  - Delete old implementation
  - Clean up include folders, e.g. <synth/io> vs <synth/device>
  - Reduce libraries ?
    - In the shared folder, e.g. have a `synth`, `adapter` and `common` library only with subfolders
  - Clean up cmake files
    - link libraries (public vs private)
    - link libraries order
  - Move content from README to doc, except for short hint how to get started

- midi: `powf` -> `<cmath>` vs `<pico/float.h>`

- Reset function for synth

  - Reset on {0xff, 0xff} independent of data or command? That would disable
    value 0xffff for all register
  - Define a reset command and just send it twice to handle case of spi out of
    sync?

- Smooth oscillator frequency change (continue with current level)
- Smooth trigger when not yet released
- Investigate triangle wave (cap instead of sharp tip)

- Add tests running on target (verify calculations)
