
#include <MMU.h>

#include "MMU.h"
#include <iostream>

u8 MMU::read(const u16 &addr) const {
	// Mirrored 2KB of internal RAM
	if (addr < 0x2000) {
		return ram[addr & 0x7FF];
	}
	// Mirrors of NES PPU registers
	else if (0x2000 <= addr && addr <= 0x3fff) {
		u16 ppuReg = static_cast<u16>(((addr - 0x2000) % 8) + 0x2000);
		return ppu.readReg(ppuReg);
	} else if (addr == 0x4014) {
		return ppu.readReg(addr);
	} else {
		return ram[addr];
	}
}

bool MMU::write(u16 addr, u8 val) {
	// Mirrored 2KB of internal RAM
	if (addr < 0x2000) {
		ram[addr & 0x7FF] = val;
		return true;
	}
		// Mirrors of NES PPU registers
	else if (0x2000 <= addr && addr <= 0x3fff) {
		u16 ppuReg = static_cast<u16>(((addr - 0x2000) % 8) + 0x2000);
		return ppu.writeReg(ppuReg, val);
	} else if (addr == 0x4014) {
		return ppu.writeReg(addr, val);
	} else {
		ram[addr] = val;
		return true;
	}
}
