# TODO


## PPUSTATUS
Sprite overflow. The intent was for this flag to be set
whenever more than eight sprites appear on a scanline, but a
hardware bug causes the actual behavior to be more complicated
and generate false positives as well as false negatives; see
PPU sprite evaluation. This flag is set during sprite
evaluation and cleared at dot 1 (the second dot) of the
pre-render line.
Sprite 0 Hit.  Set when a nonzero pixel of sprite 0 overlaps
a nonzero background pixel; cleared at dot 1 of the pre-render
line.  Used for raster timing.

