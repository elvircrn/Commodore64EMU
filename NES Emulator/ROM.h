#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <sstream>

#include "core.h"

using std::vector;

class ROM
{
	constexpr static size_t MAX_ROM_SIZE = 1024 * 1024;

	// Number of chr and prg memory banks
	u8 mapper;
	
	// Reset vector
	u16 rst;

	vector<u8> header;

	vector<vector<u8>> chr;
	vector<vector<u8>> prg;

	u32 size;
	
public:
	ROM();
	ROM(std::ifstream &filename);

	u8& operator[](const int addr);
	u8  operator[](const int addr) const;

	inline u32 Size() { return size; }
	inline int CHRCnt() { return chr.size(); }
	inline int PRGCnt() { return prg.size(); }
	inline int Mapper() { return mapper; }

	std::string Info();

	inline u8 CHR(int bank, int addr) const { return chr[bank][addr]; }
	inline u8 PRG(int bank, int addr) const { return prg[bank][addr]; }
};


