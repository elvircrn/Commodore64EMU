#include "ROM.h"
#include "FileHandler.h"


ROM::ROM() { }

ROM::ROM(const std::string &filename)
{
	std::tie(size, header, prg, chr) = FileHandler::LoadROMData(filename);
	mapper = header[7] + (header[6] >> 4);
}

u8 & ROM::operator[](const int addr)
{
	if (addr < prg.size() * 0x4000)
		return prg[addr / 0x4000][addr % 0x4000];
	else if (addr < chr.size() * 0x2000)
		return chr[addr / 0x2000][addr % 0x2000];
	
	throw "Invalid index";
}

u8 ROM::operator[](const int addr) const
{
	if (addr < prg.size() * 0x4000)
		return prg[addr / 0x4000][addr % 0x4000];
	else if (addr < chr.size() * 0x2000)
		return chr[addr / 0x2000][addr % 0x2000];

	throw "Invalid index";
}

std::string ROM::Info()
{
	std::stringstream ss;
	ss << "Bytes: " << Size() << '\n'
		<< "Kb: " << Size() / (8 * 1024) << '\n'
		<< "CHR: " << (int)CHRCnt() << '\n'
		<< "PRG: " << (int)PRGCnt() << '\n'
		<< "Mapper: " << (int)Mapper() << '\n'
		<< "RST: " << rst << '\n';
	return ss.str();
}
