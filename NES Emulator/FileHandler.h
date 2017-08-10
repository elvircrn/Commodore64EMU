#pragma once

#include "ROM.h"
#include <string>
#include <fstream>
#include <iterator>

class FileHandler
{
	void LoadData(const std::ifstream & stream, std::vector<uint8_t>& data);
public:
	FileHandler();
	~FileHandler();

	ROM LoadCartridge(const std::string &fileName);
};

