#include "ROM.h"
#include "FileHandler.h"


ROM::ROM() { }

ROM::ROM(const std::vector<u8>& _data) : data(_data)
{
	// Calculate the reset vector
	unsigned offset = 0x8000;
	rst = u16((data[0xfffd - offset] << 8) + data[0xfffc - offset]);
}

std::vector<u8> ROM::GetData() const { return data; }

std::vector<u8>& ROM::GetData() { return data; }

ROM::ROM(const std::string &filename)
{
	std::tie(header, data) = FileHandler::LoadROMData(filename);

	size = (header.size() + data.size()) * 8;
	
	prg = header[4];
	chr = header[5];
	mapper = header[7] + (header[6] >> 4);
}

u8 & ROM::operator[](const int addr)
{
	return data[addr];
}

u8 ROM::operator[](const int addr) const
{
	return data[addr];
}

std::string ROM::Status()
{
	std::stringstream ss;
	ss << "Bits: " << Size() << '\n'
		<< "Kb: " << Size() / (8 * 1024) << '\n'
		<< "CHR: " << (int)CHR() << '\n'
		<< "PRG: " << (int)PRG() << '\n'
		<< "Mapper: " << (int)Mapper() << '\n'
		<< "RST: " << rst << '\n';
	return ss.str();
}
