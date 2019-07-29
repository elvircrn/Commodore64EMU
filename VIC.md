# Signals 

## A0-A13
Address

## D0-D11
Data

## IRQ
Raster line interrupt
Sprite collision


# Memory
* The 64KB main memory
* The 1K*4 bit Color RAM
* The 4KB character generator ROM (Char ROM)



The (extended) memory map as seen by the VIC looks like this:


 $ffff +----------+   --
       |          |
       |          |
       |          |
       |   RAM    | Bank 3
       |          |
       |          |
       |          |
 $c000 +----------+   --
       |          |
       |   RAM    |
       |          |
 $a000 +----------+ Bank 2
       | Char ROM |
 $9000 +----------+
       |   RAM    |
 $8000 +----------+   --
       |          |
       |          |
       |          |
       |   RAM    | Bank 1
       |          |
       |          |
       |          |
 $4000 +----------+   --
       |          |
       |   RAM    |
       |          |
 $2000 +----------+ Bank 0
       | Char ROM |
 $1000 +----------+
       |   RAM    |
 $0000 +----------+   --


40×25 text characters are displayed in the text based modes

320×200 or 160×200 pixels in the bitmap modes.



# Refresh Counter

DRAM refresh - not really important


# D018
https://groups.google.com/forum/#!topic/comp.sys.cbm/p7owkelfs4s

$D018 only handles character set, screen and bitmap pointing for a 16K
bank and is split into two nybbles, the upper four bits tell the VIC-II
where the screen starts (simply multiply by 1K and that's where it
starts) and the lower four control where the charset is coming from (it
ignores the lowest bit, the bank is skipped through in steps of 2K since
that's how long charsets are).

If the VIC-II is in bitmap mode, only the highest bit of the lower four
is important, if it's set ($x8) then the upper half of the bank is used,
otherwise the lower half is chosen.  The screen handling for the colour
map remains the same as before.

If you want a charset at $2000 and a screen at $0800, then set $D018 to
$28, the $2x means "the third block of 1K in from the start of the bank"
and the $x8 means the "eighth block of 1K in" ($x9 will do the same as
$x8, remember that lower bit doesn't get noticed).

The highest place you can put the charset with just $D018 is $3800 and
the highest screen at $3C00 and because the bitmap has to be a block of
8K, only $0000-$01FF and $2000-$3FFF are available (even then, only the
second one can be *used*).

If you want to use another bank of 16K, $DD00's lowest two bits control
which bank is in use and, for some odd reason, it works like this:

%11 = bank 0 ($0000-$3FFF [1])
%10 = bank 1 ($4000-$7FFF)
%01 = bank 2 ($8000-$BFFF [1])
%00 = bank 3 ($C000-$FFFF)

When the VIC-II is looking at bank 1, anything you now tell $D018 is
offset into that bank, so the previous example of $28 is now pointing to
a screen at $4800 and a font at $6000.  Switch to bank 2 and the values
are offset to start at $8000 ($28 in $D018 will point to $8800 and $A000
for screen and font respectively) and it's obviously $C000 onwards for
bank 3 (and $28 now points to $C800 and $E000).


Examples:
If you want to have a font in bank 3 with the screen at $E000 and the
font itself at $C000, you need to set the lowest three bits of $DD00 to
%00 and put $80 into $D018.

LDA $DD00
AND #$FC
STA $DD00 ;No AND is needed, we want those lowest two bits clear.

LDA #$80
STA $D018


If you want to have a bitmapped screen in bank 1 with the colour at
$4000 and the image at $6000, you need to set the lowest three bits of
$DD00 to %10, put $08 into $D018 and throw the VIC-II into bitmap mode.

LDA $DD00
AND #$FC
ORA #$02
STA $DD00

LDA #$80
STA $D018

LDA #$3B
STA $D011


Hope that helps (and I'm writing in a hurry from memory, there *may* be
errors so someone please proof it! =-)

[1] There is a ROM font stored in banks 0 and 2, starting 4K in and
finishing 4K later (there are shadows of both the upper and lower case
fonts).  No matter what data is under that space, only the standard C=
character set shows up when you point the VIC-II at it.


