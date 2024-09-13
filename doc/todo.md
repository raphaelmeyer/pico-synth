# TODO

## Overview

- Refactor project setup

  - Clean up include folders, e.g. <synth/io> vs <synth/device>
  - Reduce libraries ?
    - In the shared folder, e.g. have a `synth`, `adapter` and `common` library only with subfolders
  - Clean up cmake files
    - link libraries (public vs private)
    - link libraries order
  - Move content from README to doc, except for short hint how to get started

- midi: `powf` -> `<cmath>` vs `<pico/float.h>`

- Knob fast rotation ux

- Investigate signals

  - Identify and remove noise and clicks

- Smooth oscillator frequency change (continue with current level)
- Smooth trigger when not yet released
- Investigate triangle wave (cap instead of sharp tip)

- Fix segv on uisim shutdown

- Add tests running on target (verify calculations)

## Filter

keywords to search for:

- convolution
- fir, iir
- ladder
- state variable filter

### Naive

    uint16_t filtered = (value * a + last_value * (65535 - a)) / 65535;
    last_value = filtered;

### State variable filter

- https://cytomic.com/technical-papers/
