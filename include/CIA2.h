#ifndef NESEMU_CIA2_H
#define NESEMU_CIA2_H

#include <iomanip>
#include <array>

#include "core.h"

class CIA2 {
	std::array<u8, 0xffu> mem;

	/**
	 * Map external addresses into internal buffer
	 * @param addr External address
	 * @return Value from internal memory @mem
	 */
	inline u8 &get(u16 addr) {
		return mem[addr - 0xDD00];
	}

	bool isTimerAEnabled() {
		return get(0xDD0D) & 1u;
	}

	bool isTimerBEnabled() {
		return ((u8) (get(0xDD0Du) >> 0x1u) & 0x1u) != 0;
	}
public:

	inline u8 write(u16 addr, u8 val) {
		// Timer control
		L_INFO(std::cout << "CIA2 write " << std::hex << std::setw(2) << std::setfill('0') << "CIA2 addr: " << (u32) addr
							<< " val: " << (u32) val << ' ' << std::bitset<8>{val} << '\n');
		get(addr) = val;
		if (addr == 0xDD0D) {

		}
		return val;
	}

	inline u8 read(u16 addr) {
		L_INFO(std::cout << "CIA2 read " << std::hex << std::setw(2) << std::setfill('0') << " addr: " << (u32) addr << '\n');
		// Timer control
		if (addr == 0xDD0D) {
			return 1u << 7u;
		}
		return get(addr);
	}

	void tick() {

	}
};

#endif //NESEMU_CIA2_H
