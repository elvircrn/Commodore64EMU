#pragma once

#include <string>
#include <fstream>
#include <iterator>
#include <tuple>

#include "ROM.h"
#include "core.h"

using std::vector;
using std::tuple;
using std::string;

class FileHandler
{
public:
	static tuple<vector<u8>, vector<u8>> LoadROMData(const string &fileName);
};

