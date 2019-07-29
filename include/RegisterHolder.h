#ifndef NESEMU_REGISTERHOLDER_H
#define NESEMU_REGISTERHOLDER_H

#include "core.h"

template<u32 addressBase, u32 size>
class RegisterHolder {
private:
	static constexpr u32 bits = size - 1;
	std::array<u8, size> mem;

public:
	inline virtual u8 get(const u16 &addr) const {
		return mem[toMem(addr)];
	}

	inline u16 get16(const u16 &addr) const {
		return mem[toMem(addr)] + (mem[toMem(addr + 1)] << 0x8u);
	}

	inline virtual u8 set(const u16 &addr, const u8 &val) {
		return mem[toMem(addr)] = val;
	}

	inline u16 normalize(u16 addr) const {
		return addressBase + (addr & bits);
	}

	inline u16 toMem(u16 addr) const {
		return normalize(addr) - addressBase;
	}
};

#endif //NESEMU_REGISTERHOLDER_H
