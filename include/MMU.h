#ifndef NESEMU_MMU_H
#define NESEMU_MMU_H

#include <utility>
#include <vector>
#include "core.h"
#include "ROM.h"

#include <iostream>
class MMU {
	constexpr static int RAM_SIZE = 65536;

	std::vector<u8> ram;
	ROM &rom;
	std::array<u8, 0x2000> cartHi;
	std::array<u8, 0x2000> cartLo;
	std::array<u8, 0x1000> vic;

public:
	explicit MMU(ROM &_rom) : ram(RAM_SIZE), rom(_rom) {
		// The default is mode 31 (no cartridge) as all latch bits are logically high (set to 1).
		// https://www.c64-wiki.com/wiki/Bank_Switching
		ram[1] = (1u << 8u) - 1u;
	}

	inline u8 read(const u16 &addr) const {
		u8 bankMask = ram[1];


		if ((addr >> 8u) == 0xdcu) {
			std::cout << "CIA1";
		}

		if ((addr >> 8u) == 0xddu) {
			std::cout << "CIA2";
		}


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

	inline bool write(u16 addr, u8 val) {
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

	inline void writeRAM(u16 addr, u8 val) {
		ram[addr] = val;
	}
	inline bool isIO(const u8 &val) const {
		// bit_3(val) = 1 && val != 100
		return ((7 & val) != 100 && BIT(val, 2));
	}

	inline bool isCharGen(const u8 &val) const {
		return (~BIT(val, 2)) & (BIT(val, 0) | BIT(val, 1));
	}

	inline bool isBASIC(const u8 &val) const {
		return BIT(val, 0) & BIT(val, 1);
	}

	inline bool isKernal(const u8 &val) const {
		return BIT(val, 1);
	}
};

#endif //NESEMU_MMU_H

