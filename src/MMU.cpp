
#include <MMU.h>

#include "MMU.h"
#include <iostream>

u8 MMU::read(const u16 &addr) const {
	// Mirrored 2KB of internal RAM
	if (addr < 0x2000) {
		return ram[addr & 0x7FF];
	} else if (addr == 0x4016) {
		return io.read4016();
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
	} else if (addr == 0x4016) {
		io.write4016(val);
	} else if (addr == 0x4017) {
		io.write4017(val);
	} else {
		ram[addr] = val;
		return true;
	}
}
