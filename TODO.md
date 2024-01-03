# TODO

- Implement control register
  - Wave form
  - Hide anything except tone generator from Channel

- Add volume register

- Change frequency value to "f * 4" for 2 bit fraction

- Reset function for synth (e.g. when spi is out of sync)
  - Reset on {0xff, 0xff} independent of data or command? That would disable
    value 0xffff for all register
  - Define a reset command and just send it twice to handle case of spi out of
    sync?

- Add knobs (ADSR, wave)
  - Where? synth or midi adapter?

- Smooth oscillator frequency change (contine with current level)
- Smooth trigger when not yet released

- Move components from exes to device/library?

- Replace demo instrument setup with something else

- Require concept for include paths?

- Refactor to prevent wrong usage
  - e.g. device init order, repeated invocation of task function etc
