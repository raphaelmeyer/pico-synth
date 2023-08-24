# Pico Synth

## Getting started

- start vscode
- install recommended plugin
- click on `><` and select `Reopen in Container`
- configure and build

Plug in a Raspberry Pi Pico (BOOTSEL) and copy the uf2 from the container to the RPi in a shell on the host.
Paths and container name may differ.

    docker cp <container>:/build/pico-synth/pico-synth.uf2 /run/media/.../RPI-RP2/

## Pico Synth

The executable of the _pico-synth_ generated in `/build/pico-synth/pico-synth.uf2`.

### Hardware

- Raspberry Pi Pico
- ada3678 (UDA1334)

### Wiring

    Pico               ada3678

    14 (GP 10)    ---- BCLK
    15 (GP 11)    ---- WSEL
    16 (GP 12)    ---- DIN

    36 (3V3 OUT)  ---- VIN
    38 (GND)      ---- GND

## Midi Adapter

The executable of the _midi-adapter_ generated in `/build/midi-adapter/midi-adapter.uf2`.
