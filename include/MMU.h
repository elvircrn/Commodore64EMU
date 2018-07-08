#ifndef NESEMU_MMU_H
#define NESEMU_MMU_H

#include <utility>
#include <vector>
#include "core.h"
#include "PPU.h"

class MMU {
	constexpr static int RAM_SIZE = 65536;

	// TODO: Refactor this out of MMU, becase we do not have ownership over this memory.
	std::vector<u8> ram;

	// We only need a pointer to PPU, because we do not actually own the PPU object.
	PPU &ppu;

public:
	MMU(PPU &_ppu) : ram(RAM_SIZE), ppu(_ppu) {}

	u8 &operator()(u16 addr) {
		// Mirrored 2KB of internal RAM
		if (addr < 0x2000)
			return ram[addr & 0x7FF];
			// Mirrors of NES PPU registers
		else if (0x2000 <= addr && addr <= 0x3fff) {
			return ppu.RdReg(((addr - 0x2000) % 8) + 0x2000);
		} else
			return ram[addr];
	}
};

#endif //NESEMU_MMU_H
