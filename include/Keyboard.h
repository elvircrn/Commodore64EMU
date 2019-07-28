#ifndef NESEMU_KEYBOARD_H
#define NESEMU_KEYBOARD_H

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_keyboard.h>
#include "core.h"

class Keyboard {
	SDL_Scancode keymap[8][8]{};

public:
	Keyboard() {
		for (auto &i : keymap) {
			for (auto &j : i) {
				j = SDL_SCANCODE_UNKNOWN;
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

	inline bool isKeyPressed(u8 kX, u8 kY) {
		return isKeyPressed(keymap[kX][kY]);
	}

	inline bool isKeyPressed(SDL_Scancode scancode) {
		auto keystate = SDL_GetKeyboardState(nullptr);
		return keystate[scancode];
	}
};

#endif //NESEMU_KEYBOARD_H
