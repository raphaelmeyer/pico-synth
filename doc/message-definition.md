# Message

## Messsage Definition

    Message = Address, Command, [ Data ] ;
    Address = Device, Oscillator ;
    Command = Trigger | Release | Set Register ;
    Trigger = '0x01' ;
    Release = '0x02' ;
    Set Register = '0x1', Register ;

    Device =      '0x0' | ... | '0xF' ;
    Oscillator =  '0x0' | ... | '0xF' ;
    Register =    '0x0' | ... | '0xF' ;
    Data =     '0x0000' | ... | '0xFFFF' ;

Remarks:

- Commands 0x00 to 0x0F have no data
- Commands 0x10 to 0xFF have data

## Oscillator Registers

    0x0   Reserved
    0x1   Frequency
    0x2   Attack
    0x3   Decay
    0x4   Sustain
    0x5   Release
    0x6   Volume
    0x7   Wave Form

## Wave Form

    Reserved:12 | Wave:4

Wave:

    0x0 Noise
    0x1 Square
    0x2 Triangle
    0x3 Sawtooth
