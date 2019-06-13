
#include <MMU.h>

#include "MMU.h"
#include <iostream>

u8 MMU::read(const u16 &addr) const {
	u8 bankMask = ram[1];

	if (addr < 0xa000) {
		return ram[addr];
	} else if (addr < 0xc000) {
		if (isBASIC(bankMask)) {
			return rom.basic[addr - 0xa000];
		} else {
			return ram[addr];
		}
	} else if (addr < 0xd000) {
		return ram[addr];
	} else if (addr < 0xe000) {
		if (isCharGen(bankMask)) {
			return rom.chargen[addr - 0xd000];
		} else if (isIO(bankMask)) {
			return rom.io[addr - 0xd000];
		}
	} else if (addr <= 0xffff) {
		if (isKernal(bankMask)) {
			return rom.kernal[addr - 0xe000];
		} else {
			return ram[addr];
		}
	}
	return ram[addr];
}

bool MMU::write(u16 addr, u8 val) {
	u8 bankMask = ram[1];

	if (addr < 0xa000) {
		return ram[addr] = val;
	} else if (addr < 0xc000) {
		if (isBASIC(bankMask)) {
			return rom.basic[addr - 0xa000] = val;
		} else {
			return ram[addr] = val;
		}
	} else if (addr < 0xd000) {
		return ram[addr] = val;
	} else if (addr < 0xe000) {
		if (isCharGen(bankMask)) {
			return rom.chargen[addr - 0xd000] = val;
		} else if (isIO(bankMask)) {
			return rom.io[addr - 0xd000] = val;
		}
	} else if (addr <= 0xffff) {
		if (isKernal(bankMask)) {
			return rom.kernal[addr - 0xe000] = val;
		} else {
			return ram[addr] = val;
		}
	}
	return ram[addr] = val;
}

void MMU::writeRAM(u16 addr, u8 val) {
	ram[addr] = val;
}
