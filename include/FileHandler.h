#pragma once

#include <string>
#include <fstream>
#include <iterator>
#include <tuple>

#include "ROM.h"
#include "core.h"
#include "optional.h"

using std::vector;
using std::tuple;
using std::string;

class FileHandler {
public:
	/**
		Returns the header, prg banks, and chr banks respectively.
	*/
	static std::experimental::optional<tuple<int,
																					 vector<u8>,
																					 vector<vector<u8>>,
																					 vector<vector<u8>>>> LoadROMData(const std::string &);
	static std::experimental::optional<tuple<int,
																					 vector<u8>,
																					 vector<vector<u8>>,
																					 vector<vector<u8>>>> LoadROMData(std::ifstream &fileStream);
};

