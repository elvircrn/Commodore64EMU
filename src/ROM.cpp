#include <utility>

#include "ROM.h"
#include "StreamUtil.h"

ROM::ROM() {}

ROM::ROM(std::ifstream &kernalStream, std::ifstream &basicStream) :
		kernal{StreamUtil::toVectorU8(kernalStream)},
		basic{StreamUtil::toVectorU8(kernalStream)} {
}

ROM::ROM(vector<u8> _kernal, vector<u8> _basic, vector<u8> _chargen)
		: kernal(std::move(_kernal)), basic(std::move(_basic)), chargen(std::move(_chargen)) {}
