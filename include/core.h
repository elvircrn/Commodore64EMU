#pragma once
#include <cstdint>
#include <string>
#include <sstream>
#include "optional.h"

typedef signed char        s8;
typedef short              s16;
typedef int                s32;
typedef long long          s64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

template<class T>
using optional = std::experimental::optional<T>;

inline u8 LO(const u16 &addr) { return static_cast<u8>(addr & 0xffu); }
inline u8 HI(const u16 &addr) { return static_cast<u8>((u8) (addr >> 0x8u) & 0xffu); }
inline bool BIT(const u8 &bits, const u8 &n) { return (bits & (1u << n)) > 0; }
inline std::string BIT_STR(u8 bits) {
	if (!bits) {
		return "0";
	}
	std::stringstream ss;
	while (bits) {
		ss << (bits & 0x1u ? '1' : '0');
		bits >>= 0x1u;
	}
	return ss.str();
}

template<class T>
inline std::string HexString(T num) {
	std::stringstream ss;
	ss << std::hex << num;
	return ss.str();
}
