#ifndef NESEMU_CIA1_H
#define NESEMU_CIA1_H

#include <iostream>
#include <iomanip>
#include <array>

#include "core.h"

class CIA1 {
	static const u16 ADDRESS_COUNTER_A = 0xDC04;
	static const u16 TIMER_CONTROL_REGISTER_A = 0xDC0E;

	std::array<u8, 0xffu> mem{};

	/**
	 * Map external addresses into internal buffer
	 * @param addr External address
	 * @return Value from internal memory @mem
	 */
	inline u8 &get(u16 addr) {
		return mem[addr - 0xDC00];
	}

	inline u16 get16(u16 addr) {
		return (get(addr + 1) << 0x8u) + get(addr);
	}

	inline void set16(u16 addr, u16 val) {
		// TODO: Check if this the right order of operations
		get(addr) = LO(val);
		get(addr + 1) = HI(val);
	}

	inline bool isTimerAEnabled() {
		return get(0xDC0D) & 1u;
	}

	inline bool isTimerBEnabled() {
		return ((u8) (get(0xDC0Du) >> 0x1u) & 0x1u) != 0;
	}

	inline u16 a() {
		return get16(ADDRESS_COUNTER_A);
	}

	inline u16 decrementA() {
		u16 timerA = a();
		set16(ADDRESS_COUNTER_A, timerA - 1);
	}

	inline u16 incrementA() {
		u16 timerA = a();
		set16(ADDRESS_COUNTER_A, timerA + 1);
	}

public:

	inline u8 write(u16 addr, u8 val) {
		// Timer control
//		std::cout << "CIA1 write " << std::hex << std::setw(2) << std::setfill('0') << "CIA1 addr: " << (u32) addr
//							<< " val: " << (u32) val << ' ' << std::bitset<8>{val} << '\n';
		get(addr) = val;
	}

	inline u8 read(u16 addr) {
//		std::cout << "CIA1 read " << std::hex << std::setw(2) << std::setfill('0') << " addr: " << (u32) addr << '\n';
		return (0xffu - 7);
		return get(addr);
	}

	void tick() {
		u8 controlA = get(TIMER_CONTROL_REGISTER_A);
		// Start timer
		if (BIT(controlA, 0)) {
			u8 counterA = a();
			decrementA();
		}
	}
};

#endif //NESEMU_CIA1_H
