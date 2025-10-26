# Chip8 Emulator coded in C with SDL3
A simple chip8 emulator made as a first foray into emulation.
It's not perfect, it does pass almost every test from Timendus's [Test Suite](https://github.com/Timendus/chip8-test-suite) except for DISP WAIT

## Usage:
Build by running Cmake and Make commands

### Command:
`./chip8 <path/to/rom.ch8> [-m]`

By using the -m flag the emulator will run in Modern mode where certain opcodes will behave differently, use it if a modern game isn't behaving as it's supposed to.

### Controls:
The chip8 used a hexadecimal input keypad. 
In the emulator these are mapped as followed (Left is your keyboard)

- 1 2 3 4  =  1 2 3 C 
- Q W E R  =  4 5 6 D 
- A S D F  =  7 8 9 E
- Z X C V  =  A 0 B F

## Known issues:
As this is my first emulator there are some issues at this moment:
- Some games(Tetris, Breakout) have some bugs with collision and such. I am not sure how much the emulator is to blame or the games.
- The display is quite flickery, I'm aware this is the nature of the Chip8 but still, I wanna look into reducing it.
- It does not feel very responive/snappy, probably has something to do with how the timing mechanism works
- Sound is not yet included

