# CHIP-8 Emulator

This repository contains a emulator for the CHIP-8 architecture/programming language (https://en.wikipedia.org/wiki/CHIP-8).
The emulator is written in C99 with a SDL backend for graphics and input.

## Build

In order to build the emulator simply run `make` in the root directory.

## Run

To run the emulator specify the rom that is to be executed:
```
$ build/chip8 <rom.ch8>
```

## References

The following references were used:
 - https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
