#ifndef NESEMU_REGISTERHOLDER_H
#define NESEMU_REGISTERHOLDER_H

#include "core.h"

template<u32 addressBase, u32 size = 0xffu>
class RegisterHolder {
private:
	std::array<u8, size> mem;

public:
	inline virtual u8 get(const u16 &addr) const {
		const u8 regAddr = ((addr - addressBase) & 0xffu);
		return mem[regAddr];
	}

	inline u16 get16(const u16 &addr) const {
		const u8 regAddr = ((addr - addressBase) & 0xffu);
		return mem[regAddr] + (mem[regAddr + 1] << 0x8u);
	}

	inline virtual u8 set(const u16 &addr, const u8 &val) {
		const u8 regAddr = ((addr - addressBase));
		return mem[regAddr] = val;
	}

	inline u16 set16(const u16 &addr, const u8 &val) {
		const u8 regAddr = ((addr - addressBase));
		mem[regAddr] = LO(val);
		mem[regAddr + 1] = HI(val);
		return val;
	}

	inline u16 normalize(u16 addr) const {
		return addressBase + (addr & size);
	}
};

#endif //NESEMU_REGISTERHOLDER_H
