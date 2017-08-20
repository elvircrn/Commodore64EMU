#include "GUI.h"

#include "SDL.h"
#include <string>

GUI::GUI() : window(nullptr), screenSurface(nullptr)
{
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw "SDL could not initialize! SDL_Error: " + std::string(SDL_GetError());

	//Create window
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Get window surface
		screenSurface = SDL_GetWindowSurface(window);

		//Fill the surface white
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

		//Update the surface
		SDL_UpdateWindowSurface(window);

		//Wait two seconds
		SDL_Delay(200000);
	}
}


GUI::~GUI()
{
	if (window != nullptr)
		SDL_DestroyWindow(window);
	SDL_Quit();
}
