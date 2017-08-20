#pragma once

#include <vector>

#include "core.h"

class PPU
{
	// Memory
	constexpr static int PPU_MEM_SIZE = 0x4000;
	std::vector<u8> mem;

	// Object attribute memory
	constexpr static int OAM_SIZE = 0x100;
	std::vector<u8> oam;

public:
	PPU();

	u8 Rd(int addr);
	u8 Wr(int addr, u8 val);
};

