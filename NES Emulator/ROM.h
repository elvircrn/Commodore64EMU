#pragma once

#include <stdint.h>
#include <vector>

#include "core.h"

using std::vector;

class ROM
{
	constexpr static size_t MAX_ROM_SIZE = 1024 * 1024;

	// Number of chr and prg memory banks
	u8 chr;
	u8 prg;
	
	// Reset vector
	u16 rst;

	vector<u8> header;
	vector<u8> data;
	
public:
	ROM();
	ROM(const std::string &filename);
	ROM(const vector<uint8_t> &_data);

	vector<uint8_t>& GetData();
	vector<uint8_t>  GetData() const;

	u8& operator[](const int addr);
	u8  operator[](const int addr) const;
};


