# TODO

- midi: `powf` -> `<cmath>` vs `<pico/float.h>`

- Refactor project setup

  - Move content from README to doc, except for short hint how to get started
  - Clean up taskfile
  - Cross compiling
  - Using find_package? For easier split of host/target executables

- Reset function for synth (e.g. when spi is out of sync)

  - Reset on {0xff, 0xff} independent of data or command? That would disable
    value 0xffff for all register
  - Define a reset command and just send it twice to handle case of spi out of
    sync?

- Smooth oscillator frequency change (contine with current level)
- Smooth trigger when not yet released

- Move components from exes to device/library?

- Require concept for include paths?

- Refactor to prevent wrong usage

  - e.g. device init order, repeated invocation of task function etc

- Add host testing with doctest or similar.

- Target test?
