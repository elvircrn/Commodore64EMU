#pragma once

#include <memory>

#include "SDL.h"

// Taken from https://eb2.co/blog/2014/04/c-plus-plus-14-and-sdl2-managing-resources/
namespace sdl2
{
	template<typename Creator, typename Destructor, typename... Arguments>
	auto make_resource(Creator c, Destructor d, Arguments&&... args);
	
	using window_ptr_t = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;

	window_ptr_t make_window(const char* title,
		int x, int y, int w, int h, Uint32 flags);
}