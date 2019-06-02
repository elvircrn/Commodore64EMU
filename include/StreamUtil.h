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

class StreamUtil {
public:
	/**
		Returns the header, prg banks, and chr banks respectively.
	*/
	static vector<u8> toVectorU8(std::ifstream &);
};

