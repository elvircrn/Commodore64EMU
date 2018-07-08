#pragma once
#include "SDL2/SDL.h"
#include "SDLCore.h"

class GUI
{
	sdl2::window_ptr_t window;
	sdl2::renderer_ptr_t renderer;
public:
	GUI();

	constexpr static int SCREEN_WIDTH  = 256;
	constexpr static int SCREEN_HEIGHT = 240;
};

