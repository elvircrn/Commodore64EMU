# CIA Chip

The following is a series of writes made by the cpu towards the cia1 and cia2 chips on start-up:

```
CIA1 write CIA1 addr: dc0d val: 7f 01111111
CIA2 write CIA2 addr: dd0d val: 7f 01111111
CIA1 write CIA1 addr: dc00 val: 7f 01111111
CIA1 write CIA1 addr: dc0e val: 8 00001000
CIA2 write CIA2 addr: dd0e val: 8 00001000
CIA1 write CIA1 addr: dc0f val: 8 00001000
CIA2 write CIA2 addr: dd0f val: 8 00001000
CIA1 write CIA1 addr: dc03 val: 0 00000000
CIA2 write CIA2 addr: dd03 val: 0 00000000
CIA1 write CIA1 addr: dc02 val: ff 11111111
CIA2 write CIA2 addr: dd00 val: 7 00000111
CIA2 write CIA2 addr: dd02 val: 3f 00111111
CIA1 write CIA1 addr: dc04 val: 95 10010101
CIA1 write CIA1 addr: dc05 val: 42 01000010
CIA1 write CIA1 addr: dc0d val: 81 10000001
CIA1 read  addr: dc0e
CIA1 write CIA1 addr: dc0e val: 11 00010001
CIA1 read  addr: dd00
CIA2 write CIA2 addr: dd00 val: 10 00010000
CIA1 read  addr: dc00
CIA1 write CIA1 addr: dc00 val: 55 01010101
CIA1 read  addr: dc00
```

Interplay between CPU and CIA chips on IRQ

```
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: bd 10111101
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: 0 00000000
CIA1 write CIA1 addr: dc00 val: 0 00000000
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: fe 11111110
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: fd 11111101
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: fb 11111011
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: f7 11110111
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: ef 11101111
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: df 11011111
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: bf 10111111
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: 7f 01111111
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: ff 11111111
CIA1 read  addr: dc01
CIA1 read  addr: dc01
CIA1 write CIA1 addr: dc00 val: 7f 01111111
CIA1 read  addr: dc0d
CIA1 write CIA1 addr: dc04 val: 25 00100101
CIA1 write CIA1 addr: dc05 val: 40 01000000
CIA1 write CIA1 addr: dc0d val: 81 10000001
CIA1 read  addr: dc0e
CIA1 write CIA1 addr: dc0e val: 11 00010001
CIA1 read  addr: dd00
CIA2 write CIA2 addr: dd00 val: 10 00010000
```
