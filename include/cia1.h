#ifndef C64EMU_CIA1_H
#define C64EMU_CIA1_H

#include <iomanip>
#include <array>
#include <utility>
#include <bitset>
#include <functional>

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_events.h>
#include <iostream>

#include "core.h"
#include "keyboard.h"
#include "log_util.h"
#include "register_holder.h"

class CIA1 : public RegisterHolder<0xDC00, 0x10> {
	static constexpr u16 ADDRESS_COUNTER_A = 0xDC04u;
	static constexpr u16 ADDRESS_COUNTER_B = 0xDC06u;
	static constexpr u16 TIMER_CONTROL_REGISTER_A = 0xDC0Eu;
	static constexpr u16 TIMER_CONTROL_REGISTER_B = 0xDC0Fu;
	static constexpr u16 INTERRUPT_CONTROL_STATUS = 0xDC0Du;

	/**
	 * DC0D Control values, read values
	 */
	inline void setTimerAUnderflow(bool val) {
		set(INTERRUPT_CONTROL_STATUS, SET(get(INTERRUPT_CONTROL_STATUS), 0, val));
	}

	inline void setTimerBUnderflow(bool val) {
		set(INTERRUPT_CONTROL_STATUS, SET(get(INTERRUPT_CONTROL_STATUS), 1, val));
	}

	inline void setInterruptGenerated(bool val) {
		set(INTERRUPT_CONTROL_STATUS, SET(get(INTERRUPT_CONTROL_STATUS), 7, val));
	}

	std::function<void()> generateInterrupt;

	u16 timerA{}, timerB{};
	bool interruptAEnabled{}, interruptBEnabled{};

	Keyboard keyboard{};

	inline bool isInterruptAEnabled() {
		return interruptAEnabled;
	}

	inline bool isInterruptBEnabled() {
		return interruptBEnabled;
	}

	inline bool isTimerAEnabled() {
		return get(INTERRUPT_CONTROL_STATUS) & 1u;
	}

	inline bool isTimerBEnabled() {
		return ((u8) (get(0xDC0Du) >> 0x1u) & 0x1u) != 0;
	}

	inline u16 timerALatch() {
		return get16(ADDRESS_COUNTER_A);
	}

	inline u16 timerBLatch() {
		return get16(ADDRESS_COUNTER_B);
	}

	inline bool isTimerAReset() {
		return BIT(get(TIMER_CONTROL_REGISTER_A), 3);
	}

	inline bool isTimerBReset() {
		return BIT(get(TIMER_CONTROL_REGISTER_B), 3);
	}

public:

	inline u8 write(u16 addr, u8 val) {
		addr = normalize(addr);

		// Timer control
		L_DEBUG(
				std::cout << "CIA1 write " << std::hex << std::setw(2) << std::setfill('0') << "CIA1 addr: " << (u32) addr
									<< " val: " << (u32) val << ' ' << std::bitset<8>{val} << '\n';
		);

		if (addr == INTERRUPT_CONTROL_STATUS) {
			// Any bits set to 1 other than the 7-th bit, get their values from the bit #7.
			bool setBit = BIT(val, 7);
			if (BIT(val, 0)) {
				interruptAEnabled = setBit;
			}
			if (BIT(val, 1)) {
				interruptBEnabled = setBit;
			}
			return (interruptAEnabled | (interruptBEnabled << 0x1u));
		} else if (addr == TIMER_CONTROL_REGISTER_A) {
			// Bit 4: 1 = Load latch into the timer once.
			if (BIT(val, 4)) {
				timerA = timerALatch();
			}
		} else if (addr == TIMER_CONTROL_REGISTER_B) {
			if (BIT(val, 4)) {
				return timerB = timerBLatch();
			}
		}
		return set(addr, val);
	}

	inline u8 read(u16 addr) {
		addr = normalize(addr);
		L_DEBUG(std::cout << "CIA1 read " << std::hex << std::setw(2) << std::setfill('0') << " addr: " << (u32) addr << ' ');

		if (addr == 0xdc01) {
			u8 pos{}, val = get(0xdc00);
			if (!val) {
				return 0x0u;
			} else if (val != 0xffu) {
				while (val & 0x1u) {
					pos++;
					val >>= 0x1u;
				}
			}

			u8 mask{};
			for (u8 i = 0; i < 0x8u; i++) {
				if (!keyboard.isKeyPressed(pos, i)) {
					mask |= 1u << i;
				}
			}

			return mask;
		} else {
			return get(addr);
		}
	}

	void tick() {
		u8 controlA = get(TIMER_CONTROL_REGISTER_A);
		u8 controlB = get(TIMER_CONTROL_REGISTER_B);

		setInterruptGenerated(false);
		setTimerAUnderflow(false);
		setTimerBUnderflow(false);

		// Start timer
		if (BIT(controlA, 0)) {
			u16 timerAPrev = timerA;
			timerA--;

			// Timer A underflow occurred.
			if (timerAPrev && !timerA) {
				if (isInterruptAEnabled()) {
					setTimerAUnderflow(true);
					setInterruptGenerated(true);
					generateInterrupt();
				}
				timerA = timerALatch();
			}
		}

		// StBrt timer
		if (BIT(controlB, 0)) {
			u16 timerBPrev = timerB;
			timerB--;

			// Timer B underflow occurred.
			if (timerBPrev && !timerB) {
				if (isInterruptBEnabled()) {
					setTimerBUnderflow(true);
					setInterruptGenerated(true);
					generateInterrupt();
				}
				timerB = timerBLatch();
			}
		}
	}

	inline void setGenerateInterrupt(std::function<void()> _generateInterrupt) {
		this->generateInterrupt = std::move(_generateInterrupt);
	}
};

#endif //C64EMU_CIA1_H
