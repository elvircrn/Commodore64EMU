#pragma once
#include "SDL.h"
class GUI
{
	SDL_Window *window;
	SDL_Surface* screenSurface;
public:
	GUI();
	~GUI();

	constexpr static int SCREEN_WIDTH  = 256;
	constexpr static int SCREEN_HEIGHT = 240;
};

