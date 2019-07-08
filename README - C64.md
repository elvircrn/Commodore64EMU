                               The area at $d000-$dfff with
                                  CHAREN=1     CHAREN=0

 $ffff +--------------+  /$e000 +----------+  +----------+
       |  Kernal ROM  | /       |  I/O  2  |  |          |
 $e000 +--------------+/  $df00 +----------+  |          |
       |I/O, Char ROM |         |  I/O  1  |  |          |
 $d000 +--------------+\  $de00 +----------+  |          |
       |     RAM      | \       |  CIA  2  |  |          |
 $c000 +--------------+  \$dd00 +----------+  |          |
       |  Basic ROM   |         |  CIA  1  |  |          |
 $a000 +--------------+   $dc00 +----------+  | Char ROM |
       |              |         |Color RAM |  |          |
       .     RAM      .         |          |  |          |
       .              .   $d800 +----------+  |          |
       |              |         |   SID    |  |          |
 $0002 +--------------+         |registers |  |          |
       | I/O port DR  |   $d400 +----------+  |          |
 $0001 +--------------+         |   VIC    |  |          |
       | I/O port DDR |         |registers |  |          |
 $0000 +--------------+   $d000 +----------+  +----------+



## CIA #1 Progress Report

### 0xDC00 
Done

### 0xDC01
Done

### 0xDC02
No need to implement

### 0xDC03
No need to implement

### 0xDC04
Simple bind

### 0xDC05
Simple bind

### 0xDC06
Simple bind
### 0xDC07
Simple bind

### 0xDC08
No need to implement
### 0xDC09
No need to implement
### 0xDC0A
No need to implement
### 0xDC0B
No need to implement



### 0xDC0C
TODO
Serial shift register. (Bits are read and written upon every positive edge of the CNT pin.)
NOTE that this likely is not needed at all, TODO: Check if this is the case!

### 0xDC0D
IMPORTANT
    Two different registers are actually used for r/w. 
Read:
    Simple bind. Requires logic to be implemented in CIA1::tick for writes
    
Write:
    Dictates how CIA#1::tick() operates e.g. interrupt enable etc.


### 0xDC0E 
TODO
Timer A control

### 0xDC0F
TODO 
Timer B control



