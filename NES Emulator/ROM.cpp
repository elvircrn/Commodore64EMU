#include "ROM.h"


ROM::ROM()
{
	data.reserve(MAX_ROM_SIZE);
}

ROM::ROM(const std::vector<uint8_t>& _data) : data(_data)
{
	unsigned offset = 0x8000;
	rst = static_cast<uint16_t>((data[0xfffd - offset] << 8) + data[0xfffc - offset]);
}

ROM::~ROM()
{
}

std::vector<uint8_t>& ROM::GetData()
{
	return data;
}

uint8_t & ROM::operator[](const int addr)
{
	return data[addr];
}

uint8_t ROM::operator[](const int addr) const
{
	return data[addr];
}
