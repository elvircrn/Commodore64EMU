#ifndef C64EMU_CIA2_H
#define C64EMU_CIA2_H

#include <iomanip>
#include <array>
#include <iostream>

#include "core.h"

class CIA2 : public RegisterHolder<0xDD00, 0x10> {
	static constexpr u16 ADDRESS_COUNTER_A = 0xDD04u;
	static constexpr u16 ADDRESS_COUNTER_B = 0xDD06u;
	static constexpr u16 TIMER_CONTROL_REGISTER_A = 0xDD0Eu;
	static constexpr u16 TIMER_CONTROL_REGISTER_B = 0xDD0Fu;
	static constexpr u16 INTERRUPT_CONTROL_STATUS = 0xDD0Du;

	std::function<void()> generateInterrupt;

	bool isTimerAEnabled() {
		return get(0xDD0D) & 1u;
	}

	bool isTimerBEnabled() {
		return ((u8) (get(0xDD0Du) >> 0x1u) & 0x1u) != 0;
	}

	u16 timerA{}, timerB{};
	bool interruptAEnabled{}, interruptBEnabled{};

	inline bool isInterruptAEnabled() {
		return interruptAEnabled;
	}

	inline bool isInterruptBEnabled() {
		return interruptBEnabled;
	}

	inline void setTimerAUnderflow(bool val) {
		set(INTERRUPT_CONTROL_STATUS, SET(get(INTERRUPT_CONTROL_STATUS), 0, val));
	}

	inline void setTimerBUnderflow(bool val) {
		set(INTERRUPT_CONTROL_STATUS, SET(get(INTERRUPT_CONTROL_STATUS), 1, val));
	}

	inline void setInterruptGenerated(bool val) {
		set(INTERRUPT_CONTROL_STATUS, SET(get(INTERRUPT_CONTROL_STATUS), 7, val));
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
				std::cout << "CIA2 write " << std::hex << std::setw(2) << std::setfill('0') << "CIA2 addr: " << (u32) addr
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
		return get(addr);
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

#endif //C64EMU_CIA2_H
