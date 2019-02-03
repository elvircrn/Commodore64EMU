#ifndef NESEMU_MMU_H
#define NESEMU_MMU_H

#include <utility>
#include <vector>
#include "core.h"
#include "PPU.h"

class MMU {
	constexpr static int RAM_SIZE = 65536;

	// TODO: Refactor this out of MMU, becase we do not have ownership over this memory.
	std::vector<u8> ram;

	// We only need a pointer to PPU, because we do not actually own the PPU object.
	PPU &ppu;

public:
	explicit MMU(PPU &_ppu) : ram(RAM_SIZE), ppu(_ppu) {}

	u8 read(const u16 &addr) const;
	bool write(u16 addr, u8 val);
};

#endif //NESEMU_MMU_H
