#include "PPU.h"

PPU::PPU() : pattern(2, vector<u8>(0x1000)),
nametables(4, vector<u8>(0x400)),
palletes(0x20)
{
}

PPU::PPU(const ROM &rom) : PPU()
{
	LoadROM(rom);
}

void PPU::LoadROM(const ROM & rom)
{
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 0x1000; j++)
			pattern[i][j] = rom.CHR(0, 0x1000 * i + j);
}

// Used for calculating https://wiki.nesdev.com/w/index.php/PPU_pattern_tables
vector<vector<u8>> PPU::Pattern()
{
	vector<vector<u8>> data(64, vector<u8>(64));

	for (int x = 0; x < 8; x++)
	for (int y = 0; y < 8; y++)
	for (int i = 0; i < 8; i++)
	for (int j = 0; j < 8; j++)
		data[x * 8 + i][y * 8 + j] = (BIT(pattern[0][(x * 8 + y) + i + 8], j) << 1) + 
		BIT(pattern[0][(x * 8 + y) + i], j);
	return data;
}

