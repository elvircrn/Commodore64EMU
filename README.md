
# Yet another NES Emulator written in C++

![Grunt status](https://ci.appveyor.com/api/projects/status/github/elvircrn/NESEmu?branch=master&svg=true)
[![Build Status](https://travis-ci.org/elvircrn/NESEmu.svg?branch=master)](https://travis-ci.org/elvircrn/NESEmu)

![6502 Schematic](6502.jpg)

# Build

For some god-forsaken reason, gcc builds fail both on Mac and Windows. MSVC and clang should
work as expected, bearing in mind the specifics of each one's C++ implementation.

# Windows
## Visual Studio(MSVC)

Visual studio has a weird cmake/conan interaction explained here:
https://docs.conan.io/en/1.3/howtos/vs2017_cmake.html
Furthermore, the contents of `.conan/profiles/default` which contain the settings that I 
used for compiling under VS and MSVC are as follows:
```
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64
build_type=Release
compiler=Visual Studio
compiler.version=15
``` 

# Mac OS X

```
sudo conan install . --install-folder=cmake-build-debug  --build=missing -s compiler=apple-clang -s compiler.version=9.1 -s compiler.libcxx=libc++ -s build_type=Debug
```

# TODOs
Refactor branches into a single method

# ROM data
0 = 4E (N)\
1 = 45 (E)\
2 = 53 (S)\
3 = 1A (Character Break, necessary!)

(Bytes 4->8-15 Can BE MODIFIED!)\
4 = PRG (Hex number depends on size of PRG file)\
5 = CHR (Hex number depends on size of CHR file)\
6 = Mapper 0-15/ V or H Mirroring, Battery, 4 Screen VRAM, Trainer Switches
    Also is used as the "Ones" place holder digit for the extended iNES Header
    format. (See Sect. 2)\
7 = Mappers 16-?, the extended iNES Header format "Tens" place holder digit.
    Again (See Sect. 2 for details on how this is set up.)\
8->15 = Not used at this tume but MUST BE ALL ZEROS or games will not work.


# PPU
## Registers
    0 = Always zero              1 = Always 1
    - = Not used                 ? = Unknown

    +---------+----------------------------------------------------------+
    | Address | Description                                              |
    +---------+----------------------------------------------------------+
    |  $2000  | PPjkU Control Register #1 (W)                   [PPUCNT0]  |
    |         |   %vMsbpiNN                                              |
    |         |               v = Execute NMI on VBlank                  |
    |         |                      1 = Enabled                         |
    |         |               M = PPU Selection (unused)                 |
    |         |                      0 = Master                          |
    |         |                      1 = Slave                           |
    |         |               s = Sprite Size                            |
    |         |                      0 = 8x8                             |
    |         |                      1 = 8x16                            |
    |         |               b = Background Pattern Table Address       |
    |         |                      0 = $0000 (VRAM)                    |
    |         |                      1 = $1000 (VRAM)                    |
    |         |               p = Sprite Pattern Table Address           |
    |         |                      0 = $0000 (VRAM)                    |
    |         |                      1 = $1000 (VRAM)                    |
    |         |               i = PPU Address Increment                  |
    |         |                      0 = Increment by 1                  |
    |         |                      1 = Increment by 32                 |
    |         |              NN = Name Table Address                     |
    |         |                     00 = $2000 (VRAM)                    |
    |         |                     01 = $2400 (VRAM)                    |
    |         |                     10 = $2800 (VRAM)                    |
    |         |                     11 = $2C00 (VRAM)                    |
    |         |                                                          |
    |         | NOTE: Bit #6 (M) has no use, as there is only one (1)    |
    |         |       PPU installed in all forms of the NES and Famicom. |
    +---------+----------------------------------------------------------+
    |  $2001  | PPU Control Register #2 (W)                   [PPUCNT1]  |
    |         |   %fffpcsit                                              |
    |         |             fff = Full Background Colour                 |
    |         |                    000 = Black                           |
    |         |                    001 = Red                             |
    |         |                    010 = Blue                            |
    |         |                    100 = Green                           |
    |         |               p = Sprite Visibility                      |
    |         |                      1 = Display                         |
    |         |               c = Background Visibility                  |
    |         |                      1 = Display                         |
    |         |               s = Sprite Clipping                        |
    |         |                      0 = Sprites not displayed in left   |
    |         |                          8-pixel column                  |
    |         |                      1 = No clipping                     |
    |         |               i = Background Clipping                    |
    |         |                      0 = Background not displayed in     |
    |         |                          left 8-pixel column             |
    |         |                      1 = No clipping                     |
    |         |               t = Display Type                           |
    |         |                      0 = Colour display                  |
    |         |                      1 = Mono-type (B&W) display         |
    +---------+----------------------------------------------------------+

### PPUCTRL0 (PPUCTRL)
* Selects the Name table to display
* Sets the ppu address increment (for reading/writing)
* Sets the address within the pattern table for sprite tiles
* Sets the address within the pattern table for background tiles
* Selects the sprite size (8x8 or 8x16)
* Sets whether to execute an interupt when drawing sprite 0
* Sets whether to execute an interupt during the Vblank period

### PPUCTRL1 (PPUMASK)
* Sets the disply to color or mono-tone
* Sets whether to clip the left 8 pixels of the background
* Sets whether to clip sprites within the left 8 pixels of the background
* Sets whether to display the screen or not
* Selects the screen background color (black, red, blue, green)

### PPUSTATUS
* Returns whether PPU is in a Vblank period
* Returns whether there are more than 8 sprites on the current scanline
* Returns whether sprite 0 has been drawn on the current scanline


# Notes on cycle counting:

Every instruction takes at least two cycles.

## Base cycle costs
immediate     +0\
zeropage      +1\
zeropage,x    +2\
zeropage,y    +2\
absolute      +2\
absolute,x    +2\
absolute,y    +2\
(indirect,x)  +4\
(indirect),y  +3

## OAM
The OAM (Object Attribute Memory) is internal memory inside the PPU that 
contains a display list of up to 64 sprites, where each sprite's information 
occupies 4 bytes.

## NOP

## Relative Addressing Mode
Only used by branches, no need to call Tick().

## Instructions that call Tick() directly:
PHP +1\
PHA +1\
PLA +2\
PLP +2

## Register transfers
Instructions starting with T do not require any extra cycles!

## Branches
Branches take an extra cycle if the branch is taken and another if zero is
crossed(not calculated).

## Flag clears
Do not take any aditional cycles.

## Accumulator addressing
Any instruction that uses this mode does not take any additional cycles

## Sets
Do not take any aditional cycles.

## Stores
Not affected by zeropage crossing
zeropage   +1\
zeropage,x +2\
zeropage,y +2\
absolute   +2\
(indirect,x) +4\
\
absolute,x   +3\
absolute,y   +3\
(indirect),y +4

## Returns
4 additional cycles.
RTI -> PLP(Pop8)  +2
       Pop16      +2?
RTS -> Pop16 +2

## BIT
zeropage +1\
absolute +2

## Compares
immediate +0\
zeropage  +1\
absolute  +2

### CMP
zeropage,x +2\
absolute,x [+2]\
absolute,y [+2]\
(indirect,x) +4\
(indirect),y [+3]


## Incremenets Decrements
Not affected by zeropage crossing

zeropage +3\
zeropage,x +4\
absolute +4\
absolute,x +5

INX, INY, DEX and DEY do not require any additional cycles.

## Jumps
### JMP
absolute +1\
indirect +3

### JSR
absolute +4

## Loads
immediate     +0\
zeropage      +1\
zeropage,x    +2\
absolute      +2\
absolute,x    [+2]\
absolute,y    [+2]\
(indirect,x)  +4\
(indirect),y  [+3]

## Shifts and rotations
accumulator   +0\
zeropage      +3\
zeropage,s    +4\
absolute      +4\
absolute,x    +5

## ADC, SBC, EOR, ORA, AND
immediate     +0\
zeropage      +1\
zeropage,x    +2\
absolute      +2\
absolute,x    [+2]\
absolute,y    [+2]\
(indirect,x)  +4\
(indirect),y  [+3]
