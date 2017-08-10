#include "FileHandler.h"

FileHandler::FileHandler()
{
}


FileHandler::~FileHandler()
{
}

ROM FileHandler::LoadCartridge(const std::string &filename)
{
	std::ifstream file(filename, std::ios::binary);

	file.unsetf(std::ios::skipws);

	ROM rom;

	auto fileIter = std::istream_iterator<uint8_t>(file);
	std::advance(fileIter, 16);

	rom.GetData().insert(rom.GetData().begin(),
		fileIter,
		std::istream_iterator<uint8_t>());

	return rom;
}
