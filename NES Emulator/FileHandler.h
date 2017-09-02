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
	/**
		Returns the header, prg banks, and chr banks respectively.
	*/
	static tuple<int, vector<u8>, vector<vector<u8>>, vector<vector<u8>>> LoadROMData(const string &fileName);
};

