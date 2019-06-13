#ifndef NESEMU_MMU_H
#define NESEMU_MMU_H

#include <utility>
#include <vector>
#include "core.h"
#include "ROM.h"

class MMU {
	constexpr static int RAM_SIZE = 65536;

	std::vector<u8> ram;
	ROM &rom;
	std::array<u8, 0x2000> cartHi;
	std::array<u8, 0x2000> cartLo;
	std::array<u8, 0x1000> vic;

public:
	explicit MMU(ROM &_rom) : rom(_rom), ram(RAM_SIZE) {
		// The default is mode 31 (no cartridge) as all latch bits are logically high (set to 1).
		// https://www.c64-wiki.com/wiki/Bank_Switching
		ram[1] = (1 << 8) - 1;
	}

	u8 read(const u16 &addr) const;
	bool write(u16 addr, u8 val);
	void writeRAM(u16 addr, u8 val);

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

