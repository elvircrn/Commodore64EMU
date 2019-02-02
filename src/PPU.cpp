
#include <PPU.h>

#include "PPU.h"

PPU::PPU() : pattern(2, vector<u8>(0x1000)),
						 nametables(4, vector<u8>(0x400)),
						 palletes(0x20),
						 oam(256){
}

PPU::PPU(const ROM &rom) : PPU() {
	LoadROM(rom);
}

bool PPU::isRendering() {
	return false;
}

void PPU::LoadROM(const ROM &rom) {
	// Pattern table mapping
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 0x1000; j++)
			pattern[i][j] = rom.CHR(0, 0x1000 * i + j);

	// Nametables
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 0x400; j++) {
			nametables[i][j] = rom.PRG(0, 0x400 * i + j);
//			std::cout << (int) nametables[i][j] << ' '; TODO: Remove
		}
//		std::cout << '\n'; TODO: Remove
	}
}

// Used for calculating https://wiki.nesdev.com/w/index.php/PPU_pattern_tables
vector<vector<u8>> PPU::Pattern(int offX, int offY) {
	vector<vector<u8>> data(64, vector<u8>(64));

	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
					// Flipping between 0 and 1 yields completely different results
					data[x * 8 + i][63 - (y * 8 + j)] = static_cast<unsigned char>((BIT(pattern[1][(x * 8 + y * 64) + i + 8 + offX], j) << 1));
//							BIT(pattern[1][(x * 8 + y) + i + offY], j));
	return data;
}

void PPU::Cycle() { }

vector<vector<u8>> PPU::Nametable(int, int) {
	vector<vector<u8>> data(30, vector<u8>(30));
	return data;
}

