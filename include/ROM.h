#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <sstream>

#include "core.h"
#include <array>

using std::vector;
using std::array;

class ROM {
    const static u8 PRG_ROM_HEADER_SIZE = 0x2u;
    u32 offset;
public:
	ROM();
	ROM(std::ifstream &kernalFile, std::ifstream &basicFile);
	ROM(vector<u8> kernal, vector<u8> basic, vector<u8> chargen, vector<u8> io);
    ROM(vector<u8> kernal, vector<u8> basic, vector<u8> chargen, vector<u8> io, vector<u8> customROM);

    inline bool isAddrCustom(u16 addr) {
        if (!hasCustom) {
            return false;
        } else {
            return offset <= addr && addr <= offset + customROM.size();
        }
    }

    inline u8 customROMByte(u16 addr) {
        return customROM[addr - offset + PRG_ROM_HEADER_SIZE];
    }

	vector<u8> kernal;
	vector<u8> basic;
	vector<u8> chargen;
	vector<u8> io;
    vector<u8> customROM;
    bool hasCustom{};
};


