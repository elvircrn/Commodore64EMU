#include "PPU.h"

PPU::PPU() : pattern(2, vector<u8>(0x1000)),
			nametables(4, vector<u8>(0x400)),
			palletes(0x20)
{
}
