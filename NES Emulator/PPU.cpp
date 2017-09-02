#include "PPU.h"

PPU::PPU() : pattern(2, vector<u8>(0x1000)),
			nametables(4, vector<u8>(0x400)),
			palletes(0x20)
{
}

void PPU::LoadROM(const ROM & rom)
{
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 0x2000; j++)
			pattern[i][j] = rom.CHR(i, j);
}
