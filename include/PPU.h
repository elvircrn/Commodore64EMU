#pragma once

#include <vector>
#include <chrono>
#include <iostream>

using std::vector;

#include "core.h"
#include "ROM.h"
#include "sdl2.h"

enum class MirroringTypes {
	Any,
	Vertical,
	Horizontal,
	MapperSwitch,
	Various
};

class PPU {
	// Memory
	constexpr static int PPU_MEM_SIZE = 0x4000;
	vector<u8> mem;

	// Object attribute memory
	constexpr static int OAM_SIZE = 0x100;
	vector<u8> oam;

	vector<vector<u8>> pattern;
	vector<vector<u8>> nametables;
	vector<u8> palletes;

#pragma region Registers
	// Controls whether the background and sprites use the left half ($0000-$0FFF) or
	// the right half ($1000-$1FFF) of the pattern table.
	u8 PPUCTRL;     // VPHB SINN
	u8 PPUMASK;     // BGRs bMmG
	u8 PPUSTATUS;   // VSO- ----
	u8 OAMADDR;     // aaaa aaaa
	u8 OAMDATA;     // dddd dddd
	u8 PPUSCROLL;   // xxxx xxxx
	u8 PPUADDR;     // aaaa aaaa
	u8 PPUDATA;     // dddd dddd
	u8 OAMDMA;      // aaaa aaaa
#pragma endregion

public:
	static constexpr std::chrono::milliseconds CYCLE_LENGTH{500};
#pragma region Constructors
	PPU();
	PPU(const ROM &rom);
#pragma endregion

	inline u8 &RdReg(u16 addr) {
		if (addr == 0x2000)
			return PPUCTRL;
		else if (addr == 0x2001)
			return PPUMASK;
		else if (addr == 0x2002)
			return PPUSTATUS;
		else if (addr == 0x2003)
			return OAMADDR;
		else if (addr == 0x2004)
			return OAMDATA;
		else if (addr == 0x2005)
			return PPUSCROLL;
		else if (addr == 0x2006)
			return PPUADDR;
		else if (addr == 0x2007)
			return PPUDATA;
		else if (addr == 0x4014)
			return OAMDMA;
		throw "Register not found";
	}

	inline u8 &Rd(u16 addr) {
		// Registers
		if (addr < 0x2000)
			return pattern[addr / 0x1000][addr & 0xfff];
		else if (addr < 0x3000)
			return nametables[(addr - 0x2000) / 0x400][addr];
		else if (addr < 0x3fff)
			return Rd(addr - (u8) 0x1000);
		else if (addr < 0x3f20)
			return palletes[addr - 0x3f00];
		else if (addr < 0x4000)
			return Rd(addr - (u8) 0xe0);
		return mem[addr];
	}

	inline u8 Rd(u16 addr) const {
		// Registers
		if (addr < 0x2000)
			return pattern[addr / 0x1000][addr & 0xfff];
		else if (addr < 0x3000)
			return nametables[(addr - 0x2000) / 0x400][addr];
		else if (addr < 0x3fff)
			return Rd(addr - 0x1000);
		else if (addr < 0x3f20)
			return palletes[addr - 0x3f00];
		else if (addr < 0x4000)
			return Rd(addr - 0xe0);
		return mem[addr];
	}

	inline u8 RdReg(u16 addr) const {
		switch (addr) {
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

	inline u8 &operator[](u16 addr) { return Rd(addr); }
	inline u8 operator[](u8 addr) const { return Rd(addr); }

	void LoadROM(const ROM &rom);

	vector<vector<u8>> Pattern(int, int);
	vector<vector<u8>> Nametable(int, int);

	void Cycle();

	// PPU Control registers mappings
	inline bool isNMI() const { return BIT(PPUCTRL, 7); }
	inline u8 increment() const { return BIT(PPUCTRL, 2) ? 32 : 1; }

	/**
		Rendering is fetched 8 BG pixels at a time, as follows:
			* Nametable fetch
			* Attribute table fetch (which palette)
			* Pattern table fetch, plane 0
			* Pattern table fetch, plane 1

		Conceptually, the PPU does this 33 times for each scanline:
			* Fetch a nametable entry from $2000-$2FBF.
			* Fetch the corresponding attribute table entry from $23C0-$2FFF and increment the current VRAM address within the same row.
			* Fetch the low-order byte of an 8x1 pixel sliver of pattern table from $0000-$0FF7 or $1000-$1FF7.
			* Fetch the high-order byte of this sliver from an address 8 bytes higher.
			* Turn the attribute data and the pattern table data into palette indices, and combine them with data from sprite data using priority.
			* It also does a fetch of a 34th (nametable, attribute, pattern) tuple that is never used, but some mappers rely on this fetch for timing purposes.
	 */
	void drawStuff() {
		// Fetch a nametable entry from $2000-$2FBF.
		for (u16 nametableAddr = 0x2000; nametableAddr < 0x2FC0; nametableAddr++) {
			u8 nametable = Rd(nametableAddr);
			u16 patternTableAddr = (nametableAddr - 0x2000) / 16;

			// Generate pattern tables
			for (u8 offset = 0; offset < 8; offset++) {
				u8 lower = Rd(patternTableAddr + offset);
				u8 higher = Rd(patternTableAddr + offset + 8);
				for (int i = 0; i < 8; i++) {
					u8 pattern = (BIT(higher, i) << 1) | BIT(lower, i);
				}
			}

			u8 patternLow = Rd(patternTableAddr), patternHigh = Rd(patternTableAddr + 8);

			u8 p = (static_cast<u8>(BIT(patternHigh, 7) << 1)) | BIT(patternLow, 7); // p is in [0, 3]

			u8 patternVal = (patternHigh << 8) + patternLow;
		}
		std::cout << '\n';
	}

	// NOTE: Only for debugging pruposes
	std::vector<std::vector<char>> getPatternTable(u16 offset) const {
		std::vector<std::vector<char>> ret(128, std::vector<char>(128));
		// The pattern table is divided into two 256-tile sections: $0000-$0FFF, nicknamed "left", and $1000-$1FFF, nicknamed
		int x{}, y{}, id{};
		for (u16 addr = offset; addr < offset + 0x1000; addr += 16) {
			x = id / 16;
			y = id % 16;
			for (int i = 0; i < 8; i++) {
				u8 lower = Rd(addr + i);
				u8 higher = Rd(addr + i + 8);
				for (int j = 0; j < 8; j++) {
					u8 p = (BIT(higher, 7 - j) << 1) + BIT(lower, 7 - j);
					ret[x * 8 + i][y * 8 + j] = p;
				}
			}

			id++;
		}
		return ret;
	}
};


