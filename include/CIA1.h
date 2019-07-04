#ifndef NESEMU_CIA1_H
#define NESEMU_CIA1_H

#include <iostream>
#include <iomanip>
#include <array>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_events.h>

#include "core.h"

class CIA1 {
	static const u16 ADDRESS_COUNTER_A = 0xDC04;
	static const u16 TIMER_CONTROL_REGISTER_A = 0xDC0E;

	SDL_Event &event;

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
// Timer control
//		std::cout << "CIA1 write " << std::hex << std::setw(2) << std::setfill('0') << "CIA1 addr: " << (u32) addr
//							<< " val: " << (u32) val << ' ' << std::bitset<8>{val} << '\n';

		if (addr == 0xdc00) {
			if (val == 0xff) { // invalid state
				return get(addr) = val;
			}
		}
		get(addr) = val;
	}

	inline u8 read(u16 addr) {
//		std::cout << "CIA1 read " << std::hex << std::setw(2) << std::setfill('0') << " addr: " << (u32) addr << '\n';

	 if (addr == 0xdc01) {
		 u8 pos{}, val = get(0xdc00);
		 if (val == 0xff) {
		 	return 0;
		 }
		 while (val & 0x1u) {
			 pos++;
			 val >>= 0x1u;
		 }

		 auto keystate = SDL_GetKeyboardState(nullptr);

		 u8 mask{};
		 for (u8 i = 0; i < 0x8u; i++) {
			 if (keystate[keymap[pos][i]]) {
				 mask |= (keymap[pos][i] ? 0x1u : 0x0u) << i;
				 if (mask > 0) {
					 std::cout << "woo a keypress\n";
				 }
			 }
		 }

		 return mask;
	 }

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
