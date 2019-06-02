#include "StreamUtil.h"
#include "optional.h"

#include <iostream>
#include <sstream>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <fstream>

using std::tuple;
using std::vector;

vector<u8> StreamUtil::toVectorU8(std::ifstream &fileStream) {
	return std::vector<u8>{std::istream_iterator<u8>(fileStream), std::istream_iterator<u8>()};
}
