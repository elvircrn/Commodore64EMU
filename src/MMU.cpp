
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
	} else if (addr == 0x4016) {
		return 1;
//		return io.read4016();
	} else if (addr == 0x4017) {
		return io.read4017();
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
	} else if (addr == 0x4016) {
		io.write4016(val);
	} else if (addr == 0x4017) {
		io.write4017(val);
	} else {
		ram[addr] = val;
		return true;
	}
}
