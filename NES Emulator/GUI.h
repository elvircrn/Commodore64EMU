#pragma once
#include "SDL.h"
#include "SDLCore.h"

class GUI
{
	sdl2::window_ptr_t window;
	SDL_Surface* screenSurface;

	int sdlState;
public:
	GUI();

	constexpr static int SCREEN_WIDTH  = 256;
	constexpr static int SCREEN_HEIGHT = 240;
};

