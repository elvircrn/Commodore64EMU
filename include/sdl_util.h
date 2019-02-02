#ifndef NESEMU_SDL_UTIL_H
#define NESEMU_SDL_UTIL_H

#include <SDL2/SDL.h>

void putPixelRGB(SDL_Renderer *renderer, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
	SDL_SetRenderDrawColor(renderer, (Uint8) r, (Uint8) g, (Uint8) b, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}

#endif //NESEMU_SDL_UTIL_H
