# Pico Synth

## Getting started

- start vscode
- install recommended plugin
- click on `><` and select `Reopen in Container`
- configure and build

Plug in a Raspberry Pi Pico (BOOTSEL) and copy the uf2 from the container to
the RPi in a shell on the host. Paths and container name may differ.

    docker cp <container>:/build/executable/.../<...>.uf2 /run/media/.../RPI-RP2/

## Pico Synth

The executable of the _pico-synth_ is generated in
`/build/executable/pico-synth/pico-synth.uf2`.

### Hardware

- Raspberry Pi Pico
- ada3678 (UDA1334)

### Wiring

#### I2S (ada3678)

    14 (GP 10)    ---- BCLK
    15 (GP 11)    ---- WSEL
    16 (GP 12)    ---- DIN

    36 (3V3 OUT)  ---- VIN
    38 (GND)      ---- GND

#### Control (SPI slave)

    21 (GP 16)  MOSI
    22 (GP 17)  CS
    24 (GP 18)  SCK
    25 (GP 19)  MISO

## Midi Adapter

The executable of the _midi-adapter_ is generated in
`/build/executable/midi-adapter/midi-adapter.uf2`.

### Hardware

- Raspberry Pi Pico
- ada2088 (ST7735R)

### Wiring

#### Control (SPI master)

    16 (GP 12)  MISO
    17 (GP 13)  CS
    19 (GP 14)  SCK
    20 (GP 15)  MOSI

#### Display

    22 (GP 17)    ---- TCS
    24 (GP 18)    ---- SCK
    25 (GP 19)    ---- SI
    26 (GP 20)    ---- D/C
    27 (GP 21)    ---- RST

    36 (3V3 OUT)  ---- VIN
    38 (GND)      ---- GND

## Target tests

Available target tests:
    - `/build/library/specs/library-specs.uf`

Conncet to the USB serial device, e.g. with `python-pyserial`:

    python -m serial.tools.miniterm /dev/ttyACM0

A device flashed with tests can accepts requests from `picotool` by using flag
`-f`.
