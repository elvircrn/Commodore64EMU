#include "StreamUtil.h"

#include <iterator>

using std::tuple;
using std::vector;

vector<u8> StreamUtil::toVectorU8(std::ifstream &fileStream) {
	return std::vector<u8>{std::istream_iterator<u8>(fileStream), std::istream_iterator<u8>()};
}
