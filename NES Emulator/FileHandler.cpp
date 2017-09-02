#include "FileHandler.h"

using std::tuple;
using std::vector;

tuple<int, vector<u8>, vector<vector<u8>>, vector<vector<u8>>> 
	FileHandler::LoadROMData(const std::string &filename)
{
	std::ifstream file(filename, std::ios::binary);
	file.unsetf(std::ios::skipws);
	auto fileIter = std::istream_iterator<u8>(file);

	vector<u8> header;
	 // The first 16 bytes of NES ROMs correspond to the header.
	for (int i = 0; i < 16; i++)
		header.push_back(*(fileIter++));

	vector<vector<u8>> prg(header[4]), chr(header[5]);

	int size = 0;

	for (int i = 0; i < prg.size(); i++)
		for (int j = 0; j < 0x4000; j++, size++)
			prg[i].push_back(*(fileIter++));

	for (int i = 0; i < chr.size(); i++)
		for (int j = 0; j < 0x2000; j++, size++)
			chr[i].push_back(*(fileIter++));

	return { size, header, prg, chr };
}
