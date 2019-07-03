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

