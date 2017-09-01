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
	u8 chr;
	u8 prg;
	u8 mapper;
	
	// Reset vector
	u16 rst;

	vector<u8> header;
	vector<u8> data;

	u32 size;
	
public:
	ROM();
	ROM(const std::string &filename);
	ROM(const vector<uint8_t> &_data);

	vector<uint8_t>& GetData();
	vector<uint8_t>  GetData() const;

	u8& operator[](const int addr);
	u8  operator[](const int addr) const;

	inline u32 Size() { return size; }
	inline u8 CHR() { return chr; }
	inline u8 PRG() { return prg; }
	inline u8 Mapper() { return mapper; }

	std::string Status();
};


