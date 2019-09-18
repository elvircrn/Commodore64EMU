#ifndef C64EMU_CIA2_H
#define C64EMU_CIA2_H

#include <iomanip>
#include <array>

#include "core.h"

class CIA2 : public RegisterHolder<0xDD00, 0x10> {
	bool isTimerAEnabled() {
		return get(0xDD0D) & 1u;
	}

	bool isTimerBEnabled() {
		return ((u8) (get(0xDD0Du) >> 0x1u) & 0x1u) != 0;
	}
public:

	inline u8 write(u16 addr, u8 val) {
		// Timer control
		L_DEBUG(std::cout << "CIA2 write " << std::hex << std::setw(2) << std::setfill('0') << "CIA2 addr: " << (u32) addr
											<< " val: " << (u32) val << ' ' << std::bitset<8>{val} << '\n');
		set(addr, val);
		if (addr == 0xDD0D) {

		}
		return val;
	}

	inline u8 read(u16 addr) {
		L_DEBUG(
				std::cout << "CIA2 read " << std::hex << std::setw(2) << std::setfill('0') << " addr: " << (u32) addr << '\n');
		// Timer control
		if (addr == 0xDD0D) {
			return 1u << 7u;
		}
		return get(addr);
	}
};

#endif //C64EMU_CIA2_H
