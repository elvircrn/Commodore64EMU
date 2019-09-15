#include <utility>

#include "ROM.h"
#include "StreamUtil.h"

ROM::ROM() {}

ROM::ROM(std::ifstream &kernalStream, std::ifstream &basicStream) :
        kernal{StreamUtil::toVectorU8(kernalStream)},
        basic{StreamUtil::toVectorU8(kernalStream)} {
}

ROM::ROM(vector<u8> _kernal, vector<u8> _basic, vector<u8> _chargen, vector<u8> _io)
        : kernal(std::move(_kernal)), basic(std::move(_basic)), chargen(std::move(_chargen)), io(std::move(_io)) {}

ROM::ROM(vector<u8> _kernal, vector<u8> _basic, vector<u8> _chargen, vector<u8> _io, vector<u8> _customROM)
        : kernal(std::move(_kernal)), basic(std::move(_basic)), chargen(std::move(_chargen)), io(std::move(_io)), customROM(std::move(_customROM)) {
    offset = (customROM[1] << 0x8u) + customROM[0];
}

