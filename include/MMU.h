#ifndef NESEMU_MMU_H
#define NESEMU_MMU_H

#include <utility>
#include <vector>
#include "core.h"
#include "IO.h"
#include "ROM.h"

class MMU {
	constexpr static int RAM_SIZE = 65536;

	// TODO: Refactor this out of MMU, becase we do not have ownership over this memory.
	IO &io;
	std::vector<u8> ram;
	ROM rom;
	std::array<u8, 0x2000> cartHi;
	std::array<u8, 0x2000> cartLo;
	std::array<u8, 0x1000> vic;

public:
	explicit MMU(IO &_io, ROM &_rom) : io(_io), rom(_rom), ram(RAM_SIZE) {
		ram[1] = 7;
	}

	u8 read(const u16 &addr) const;
	bool write(u16 addr, u8 val);

	inline bool isCharGen(const u8 &val) const {
		return ~BIT(val, 2) & (BIT(val, 0) | BIT(val, 1));
	}

	inline bool isBASIC(const u8 &val) const {
		return BIT(val, 0) & BIT(val, 1);
	}

	inline bool isKernal(const u8 &val) const {
		return BIT(val, 1);
	}
};

#endif //NESEMU_MMU_H

