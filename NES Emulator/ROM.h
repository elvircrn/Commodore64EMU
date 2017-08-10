#pragma once

#include <stdint.h>
#include <vector>

class ROM
{
	constexpr static size_t MAX_ROM_SIZE = 1024 * 1024;
	std::vector<uint8_t> data;
	uint16_t rst;
	
public:
	ROM();
	ROM(const std::vector<uint8_t> &_data);
	~ROM();

	std::vector<uint8_t>& GetData();

	uint8_t& operator[](const int addr);
	uint8_t operator[](const int addr) const;
};


