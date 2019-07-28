#ifndef NESEMU_CIA1_H
#define NESEMU_CIA1_H

#include <iomanip>
#include <array>
#include <utility>

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_events.h>

#include "core.h"
#include "Keyboard.h"
#include "LogUtil.h"
#include "RegisterHolder.h"

class CIA1 : public RegisterHolder<0xDC00> {
	/**
	 * Addresses
	 */
	static const u16 ADDRESS_COUNTER_A = 0xDC04u;
	static const u16 ADDRESS_COUNTER_B = 0xDC06u;
	static const u16 TIMER_CONTROL_REGISTER_A = 0xDC0Eu;
	static const u16 TIMER_CONTROL_REGISTER_B = 0xDC0Fu;
	static const u16 INTERRUPT_CONTROL_STATUS = 0xDC0Du;

	/**
	 * DC0D Control values, read values
	 */
	// Bit #0
	void setTimerAUnderflow(bool val) {
		get(INTERRUPT_CONTROL_STATUS) |= (0xffu & (u8(val << 0x0u)));
	}

	// Bit #1
	void setTimerBUnderflow(bool val) {
		get(INTERRUPT_CONTROL_STATUS) |= (0xffu & (u8(val << 0x1u)));
	}

	// Bit #2
	void setTimerTODEqual(bool val) {
		get(INTERRUPT_CONTROL_STATUS) |= (0xffu & (u8(val << 0x2u)));
	}

	// Bit #3
	void setShiftRegisterOverflow(bool val) {
		get(INTERRUPT_CONTROL_STATUS) |= (0xffu & (u8(val << 0x3u)));
	}

	// Bit #4
	void setSignalLevelOnFLAGPin(bool val) {
		get(INTERRUPT_CONTROL_STATUS) |= (0xffu & (u8(val << 0x4u)));
	}

	// Bit #7
	void setInterruptGenerated(bool val) {
		get(INTERRUPT_CONTROL_STATUS) |= (0xffu & (u8(val << 0x7u)));
	}

	/**
	 * CIA1 Register values
	 */
	std::array<u8, 0x10u> mem{};

	std::function<void()> generateInterrupt;

	u16 timerA{}, timerB{};

	SDL_Event &event;

	Keyboard keyboard{};

	/**
	 * Map external addresses into internal buffer
	 * @param addr External address
	 * @return Value from internal memory @mem
	 */
	inline u8 &get(u16 addr) {
		return mem[(addr - 0xDC00u) & 0xfu];
	}

	inline u16 get16(u16 addr) {
		return (get(addr + 1) << 0x8u) + get(addr);
	}

	inline void set16(u16 addr, u16 val) {
		// TODO: Check if this the right order of operations
		get(addr) = LO(val);
		get(addr + 1) = HI(val);
	}

	inline bool isInterruptAEnabled() {
		return BIT(get(INTERRUPT_CONTROL_STATUS), 0);
	}

	inline bool isInterruptBEnabled() {
		return BIT(get(INTERRUPT_CONTROL_STATUS), 1);
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

	explicit CIA1(SDL_Event &event) : event(event) {
	}

	inline u8 write(u16 addr, u8 val) {
		addr = normalize(addr);

		// Timer control
		L_INFO(std::cout << "CIA1 write " << std::hex << std::setw(2) << std::setfill('0') << "CIA1 addr: " << (u32) addr
										 << " val: " << (u32) val << ' ' << std::bitset<8>{val} << '\n');

		if (addr == 0xdc0du) {
			if (BIT(val, 7)) {
				get(addr) = val;
			} else {
				get(addr) = 0;
			}
		} else if (addr == 0xdc0e) {
			// Bit 4: 1 = Load latch into the timer once.
			if (BIT(val, 4)) {
				timerA = timerALatch();
			}
		} else if (addr == 0xdc0f) {
			if (BIT(val, 4)) {
				timerB = timerBLatch();
			}
		}
		return get(addr) = val;
	}

	inline u8 read(u16 addr) {
		addr = (addr & 0xfu) + 0xdc00u;
		L_INFO(
				std::cout << "CIA1 read " << std::hex << std::setw(2) << std::setfill('0') << " addr: " << (u32) addr << ' ');

		// Port B, keyboard matrix rows and joystick
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
				if (keyboard.isKeyPressed(pos, i)) {
					mask |= 1u << i;
				}
			}

			if (mask > 0) {
				mask = ~mask;
			} else {
				mask = 0xffu;
			}
			return mask;
		} else if (addr == 0xdc0c) {
			// TODO: Figure out how CNT input pin works. Check if this is actually used.
		} else if (addr == 0xdc0d) {

		} else if (addr == 0xdc0e) {
		}

		return get(addr);
	}

	void tick() {
		u8 controlA = get(TIMER_CONTROL_REGISTER_A);

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
					SDL_PollEvent(&event);
					setTimerAUnderflow(true);
					setInterruptGenerated(true);
					generateInterrupt();
				}
				timerA = timerALatch();
			}
		}
	}

	inline void setGenerateInterrupt(std::function<void()> _generateInterrupt) {
		this->generateInterrupt = std::move(_generateInterrupt);
	}
};

#endif //NESEMU_CIA1_H
