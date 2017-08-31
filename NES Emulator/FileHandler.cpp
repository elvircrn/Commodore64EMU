#include "FileHandler.h"

using std::tuple;
using std::vector;

tuple<vector<u8>, vector<u8>> FileHandler::LoadROMData(const std::string &filename)
{
	vector<u8> header, data;
	std::ifstream file(filename, std::ios::binary);
	file.unsetf(std::ios::skipws);
	auto fileIter = std::istream_iterator<u8>(file);
	// The first 16 bytes of NES ROMs correspond to the header.
	for (int i = 0; i < 16; i++)
		header.push_back(*(fileIter++));
	// The rest are pushed onto the data vector
	data.insert(data.begin(), fileIter, std::istream_iterator<u8>()); 
	return { header, data };
}
