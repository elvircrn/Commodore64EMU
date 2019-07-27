#include <utility>
#include "LogUtil.h"

#ifndef NESEMU_CIA1_H
#define NESEMU_CIA1_H

#include <iostream>
#include <iomanip>
#include <array>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_events.h>

#include "core.h"

class CIA1 {
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

	SDL_Scancode keymap[8][8];

public:

	CIA1(SDL_Event &event) : event(event) {
		for (u8 i = 0; i < 8; i++) {
			for (u8 j = 0; j < 8; j++) {
				keymap[i][j] = SDL_SCANCODE_UNKNOWN;
			}
		}
		keymap[1][2] = SDL_SCANCODE_A;
		keymap[3][4] = SDL_SCANCODE_B;
		keymap[2][4] = SDL_SCANCODE_C;
		keymap[2][2] = SDL_SCANCODE_D;
		keymap[1][6] = SDL_SCANCODE_E;
		keymap[2][5] = SDL_SCANCODE_F;
		keymap[3][2] = SDL_SCANCODE_G;
		keymap[3][5] = SDL_SCANCODE_H;
		keymap[4][1] = SDL_SCANCODE_I;
		keymap[4][2] = SDL_SCANCODE_J;
		keymap[4][5] = SDL_SCANCODE_K;
		keymap[5][2] = SDL_SCANCODE_L;
		keymap[4][4] = SDL_SCANCODE_M;
		keymap[4][7] = SDL_SCANCODE_N;
		keymap[4][6] = SDL_SCANCODE_O;
		keymap[5][1] = SDL_SCANCODE_P;
		keymap[7][6] = SDL_SCANCODE_Q;
		keymap[2][1] = SDL_SCANCODE_R;
		keymap[1][5] = SDL_SCANCODE_S;
		keymap[2][6] = SDL_SCANCODE_T;
		keymap[3][6] = SDL_SCANCODE_U;
		keymap[3][7] = SDL_SCANCODE_V;
		keymap[1][1] = SDL_SCANCODE_W;
		keymap[2][7] = SDL_SCANCODE_X;
		keymap[3][1] = SDL_SCANCODE_Y;
		keymap[1][4] = SDL_SCANCODE_Z;
		keymap[7][0] = SDL_SCANCODE_1;
		keymap[7][3] = SDL_SCANCODE_2;
		keymap[1][0] = SDL_SCANCODE_3;
		keymap[1][3] = SDL_SCANCODE_4;
		keymap[2][0] = SDL_SCANCODE_5;
		keymap[2][3] = SDL_SCANCODE_6;
		keymap[3][0] = SDL_SCANCODE_7;
		keymap[3][3] = SDL_SCANCODE_8;
		keymap[4][0] = SDL_SCANCODE_9;
		keymap[4][3] = SDL_SCANCODE_0;
		keymap[0][4] = SDL_SCANCODE_F1;
		keymap[0][4] = SDL_SCANCODE_F3;
		keymap[0][4] = SDL_SCANCODE_F5;
		keymap[0][4] = SDL_SCANCODE_F7;
		keymap[0][1] = SDL_SCANCODE_RETURN;
		keymap[7][4] = SDL_SCANCODE_SPACE;
		keymap[1][7] = SDL_SCANCODE_LSHIFT;
		keymap[6][4] = SDL_SCANCODE_RSHIFT;
		keymap[5][7] = SDL_SCANCODE_COMMA;
		keymap[5][4] = SDL_SCANCODE_PERIOD;
		keymap[6][7] = SDL_SCANCODE_SLASH;
		keymap[6][2] = SDL_SCANCODE_SEMICOLON;
		keymap[6][5] = SDL_SCANCODE_EQUALS;
		keymap[0][0] = SDL_SCANCODE_BACKSPACE;
		keymap[5][3] = SDL_SCANCODE_MINUS;
		keymap[5][5] = SDL_SCANCODE_BACKSLASH; // :
		keymap[5][0] = SDL_SCANCODE_LEFTBRACKET; // +
		keymap[6][1] = SDL_SCANCODE_RIGHTBRACKET; // *
		keymap[5][6] = SDL_SCANCODE_APOSTROPHE; // @
		keymap[7][5] = SDL_SCANCODE_LGUI; // commodore key
	}

	inline u8 write(u16 addr, u8 val) {
		addr = (addr & 0xfu) + 0xdc00u;
		// Timer control
		L_INFO(std::cout << "CIA1 write " << std::hex << std::setw(2) << std::setfill('0') << "CIA1 addr: " << (u32) addr
							<< " val: " << (u32) val << ' ' << std::bitset<8>{val} << '\n');

		if (addr == 0xdc00) {
			if (val == 0xff) { // invalid state
				return get(addr) = val;
			}
		} else if (addr == 0xdc0du) {
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
		L_INFO(std::cout << "CIA1 read " << std::hex << std::setw(2) << std::setfill('0') << " addr: " << (u32) addr << ' ');

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

			auto keystate = SDL_GetKeyboardState(nullptr);

			u8 mask{};
			for (u8 i = 0; i < 0x8u; i++) {
				if (keystate[keymap[pos][i]]) {
					mask |= (keymap[pos][i] ? 0x1u : 0x0u) << i;
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
