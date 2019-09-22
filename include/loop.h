#ifndef C64EMU_LOOP_H
#define C64EMU_LOOP_H

#include "screen.h"

class Loop {
	Screen &screen;
	SDL_Event &event;
public:

	explicit Loop(Screen &_screen, SDL_Event &_event) : screen(_screen), event(_event) {}

	bool draw() {
		screen.flushTexture();
		screen.draw();
		return true;
	}

	bool update() {
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return false;
			}
		}
		return true;
	};
};

#endif //C64EMU_LOOP_H
