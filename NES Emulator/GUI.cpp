#include <iostream>
#include <string>

#include "SDL.h"
#include "GUI.h"


GUI::GUI() : window(sdl2::make_window("NESEmu",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	SCREEN_WIDTH,
	SCREEN_HEIGHT,
	SDL_WINDOW_SHOWN)),
	screenSurface(SDL_GetWindowSurface(window.get()))
{
	if (!window)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
		return;
	}

	if (!screenSurface)
	{
		std::cout << SDL_GetError() << '\n';
		return;
	}

	//Fill the surface white
	SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

	//Update the surface
	SDL_UpdateWindowSurface(window.get());

	//Wait two seconds
	SDL_Delay(200000);
}
