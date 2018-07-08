#include "MMU.h"
u8 &MMU::operator()(u16 addr) {
	// Mirrored 2KB of internal RAM
	if (addr < 0x2000)
		return ram[addr & 0x7FF];
		// Mirrors of NES PPU registers
	else if (0x2000 <= addr && addr <= 0x3fff) {
		return ppu.RdReg(((addr - 0x2000) % 8) + 0x2000);
	} else
		return ram[addr];
}
