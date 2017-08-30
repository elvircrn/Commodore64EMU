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

	u8 PPUCTRL;     // VPHB SINN
	u8 PPUMASK;     // BGRs bMmG
	u8 PPUSTATUS;   // VSO- ----
	u8 OAMADDR;     // aaaa aaaa
	u8 OAMDATA;     // dddd dddd
	u8 PPUSCROLL;   // xxxx xxxx
	u8 PPUADDR;     // aaaa aaaa
	u8 PPUDATA;     // dddd dddd
	u8 OAMDMA;      // aaaa aaaa

public:

	inline u8& Rd(u16 addr)
	{
		switch (addr)
		{
			case 0x2000: return PPUCTRL;
			case 0x2001: return PPUMASK;
			case 0x2002: return PPUSTATUS;
			case 0x2003: return OAMADDR;
			case 0x2004: return OAMDATA;
			case 0x2005: return PPUSCROLL;
			case 0x2006: return PPUADDR;
			case 0x2007: return PPUDATA;
			case 0x4014: return OAMDMA;
			default: return mem[addr];
		}
	}

	inline u8 Rd(u16 addr) const
	{
		switch (addr)
		{
			case 0x2000: return PPUCTRL;
			case 0x2001: return PPUMASK;
			case 0x2002: return PPUSTATUS;
			case 0x2003: return OAMADDR;
			case 0x2004: return OAMDATA;
			case 0x2005: return PPUSCROLL;
			case 0x2006: return PPUADDR;
			case 0x2007: return PPUDATA;
			case 0x4014: return OAMDMA;
			default: return mem[addr];
		}
	}

	inline u8& operator[](u16 addr) { return Rd(addr); }
	inline u8 operator[](u8 addr) const { return Rd(addr); }
};

