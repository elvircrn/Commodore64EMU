
#include <MMU.h>

#include "MMU.h"
#include <iostream>

u8 MMU::read(const u16 &addr) const {
	u16 hi = addr & 0xff00;
	u16 lo = addr & 0x00ff;
	if (addr > 0xDFFF) {
		if (isKernal(ram[1])) {
			return rom.kernal[addr - 0xE000];
		} else {
			return ram[addr - 0xE000];
		}
	} else if (addr > 0xCFFF) {
		if (isCharGen(ram[1])) {
			return rom.chargen[addr - 0xCFFF];
		} else { // TODO: Check if this is correct
			return vic[addr - 0xCFFF];
		}
	} else if (addr > 0xBFFF) {
		return ram[addr];
	} else if (addr > 0x9FFF) {
		if (isBASIC(ram[1])) {
			return rom.basic[addr - 0x9FFF];
		} else {
			return ram[addr - 0x9FFF];
		}
	} else {
		return ram[addr];
	}
}

bool MMU::write(u16 addr, u8 val) {
	if (addr > 0xDFFF) {
		if (isKernal(addr)) {
			rom.kernal[addr - 0xE000] = val;
		} else {
			ram[addr - 0xE000] = val;
		}
	} else if (addr > 0xCFFF) {
		if (isCharGen(ram[1])) {
			rom.chargen[addr - 0xCFFF] = val;
		} else { // TODO: I/O mapping
			ram[addr] = val;
		}
	} else if (addr > 0xBFFF) {
		ram[addr] = val;
	} else if (addr > 0x9FFF) {
		if (isBASIC(addr)) {
			rom.basic[addr - 0x9FFF] = val;
		} else {
			ram[addr - 0x9FFF] = val;
		}
	} else {
		ram[addr] = val;
	}
	return true;
}
