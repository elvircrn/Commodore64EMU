
#include <MMU.h>

#include "MMU.h"
#include <iostream>
u8 &MMU::operator()(u16 addr) {
	int found = -1;
	for (size_t i = 0; i < 256; i++) {
		if (ram[0x200 + i] > 0) {
			found = i;
		}
	}
	if (found > 0) {
		std::cout << found << '\n';
	}
	// Mirrored 2KB of internal RAM
	if (addr < 0x2000) {
		return ram[addr & 0x7FF];
	}
	// Mirrors of NES PPU registers
	else if (0x2000 <= addr && addr <= 0x3fff) {
		return ppu.RdReg(((addr - 0x2000) % 8) + 0x2000);
	} else if (addr == 0x4014) {
		return ppu.RdReg(addr);
	} else {
		return ram[addr];
	}
}

void MMU::write(u16 addr, u8 val) {
	if (addr == PPU::OAMDATA_ADDR) {
		ppu.write2004(val);
	} else if (addr == PPU::PPUADDR_ADDR) {
		ppu.write2006(val);
	}
}
