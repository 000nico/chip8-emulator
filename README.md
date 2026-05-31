# CHIP-8 Emulator

## Usage

```
chip8.exe <game>
```

Games must be in the `games/` folder — the emulator already fixes the path automatically.

```
chip8.exe "Pong (1 player).ch8"
chip8.exe Tetris.ch8
```

---

## What I learned building a CHIP-8 emulator

This is a guide of everything I learned building a CHIP-8 emulator, my first step into the world of emulation.

**Sources:**
- https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
- https://en.wikipedia.org/wiki/Opcode

An emulator allows us to natively run the assembly code of, for example, a video game console, by imitating a system and how it works internally, on another system with a completely different architecture. It's about perfectly recreating the environment so that code written for one system can run on another that has nothing in common technically.

Chip-8 is not really a system, but a Virtual Machine made in the 70's. Games written in Chip-8 could run on any system that has a Chip-8 interpreter.

Starting in the world of emulation with a Chip-8 system is probably the most convenient due to its few opcodes, and because many of its instructions are already in our CPUs.

An opcode (operation code) is an enumerated value that specifies the instruction/operation to be performed. They are commonly used in arithmetic logic units or central processing units.

When we make an emulator, we want to find as much information as possible about the system we want to emulate: how much memory it uses, how many registers the processor has, what architecture it uses, and whether we can get documentation of the instruction set. Chip-8 has:

- 4096 memory spaces (all bytes)
- The interpreter occupies the first 512
- 16 8-bit registers, from V0 to VF
- VF is the carry flag
- The address register, named I, has 12 bits and is used for many opcodes with memory operations
- A program counter
- Input from a hex keyboard
- 35 opcodes
- 64x32 monochrome display

As for the code itself:

- Initialize the emulator (PC at 0x200, load font sets, initialize everything to 0)
- Initialize the graphics library (SDL in this case)
- Load the game into memory
- Main cycle:
  - Handle opcodes and decrement timers
  - Window and keyboard message loop (key down, key up)
  - Clear the screen
  - Draw

---

## Controls

| CHIP-8 keyboard | Modern keyboard |
|:-:|:-:|
| 1 2 3 C | 1 2 3 4 |
| 4 5 6 D | Q W E R |
| 7 8 9 E | A S D F |
| A 0 B F | Z X C V |
