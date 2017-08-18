# Yet another NES Emualtor written in C++

![Grunt status](https://ci.appveyor.com/api/projects/status/github/elvircrn/NESEmu?branch=master&svg=true) 

# Notes on cycle counting:

Every instruction takes at least two cycles.

## NOP

## Relative Addressing Mode
Only used by branches, no need to call Tick().

## Instructions that call Tick() directly:
PHP +1 
PHA +1
PLA +2
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
absolute   +2

absolute,x   +3\
absolute,y   +3\
(indirect,x) +4\
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

CMP
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
JMP
absolute +1\
indirect +3

JSR
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


## Logic shifts
accumulator   +0\
zeropage      +3\
zeropage,s    +4\
absolute      +4\
absolute,x    +5

## Rotations
accumulator   +0\
zeropage      +3\
zeropage,x    +4\
absolute      +4\
absolute,x    +5

## Arithmetic shift
accumulator   +0\
zeropage      +3\
zeropage,x    +4\
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



