# Pico ADSR

## Getting started

- start vscode
- install recommended plugin
- click on `><` and select `Reopen in Container`
- configure and build

Plug in the RPi Pico (BOOTSEL) and copy the uf2 from the container to the RPi in a shell on the host.
Paths and container name may differ.

    docker cp <container>:/build/helloworld.uf2 /run/media/.../RPI-RP2/

## Hardware

- Raspberry Pi Pico
- ada3678 (UDA1334)

### Wiring

    Pico               ada3678

    14 (GP 10)    ---- BCLK
    15 (GP 11)    ---- WSEL
    16 (GP 12)    ---- DIN

    36 (3V3 OUT)  ---- VIN
    38 (GND)      ---- GND
