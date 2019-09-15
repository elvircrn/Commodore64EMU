#include <string>

#include "SDL2/SDL.h"
#include "GUI.h"
#include "LogUtil.h"

GUI::GUI() : window(sdl2::make_window("C64Emu",
                                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      SCREEN_WIDTH,
                                      SCREEN_HEIGHT,
                                      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)),
             renderer(sdl2::make_renderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) {
    if (!window) {
        L_ERROR(std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n');
        return;
    }

    //Update the surface
    SDL_UpdateWindowSurface(window.get());

    bool quit = false;
    SDL_Event event;

    // Loop while user hasn't quit
    while (!quit) {
        // Check all new event to see if a SDL_QUIT event has come in...
        while (SDL_PollEvent(&event)) {
            // SDL_QUIT event has come in, quit.
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer.get());
        SDL_SetRenderDrawColor(renderer.get(), 150, 0, 0, 255);
        SDL_RenderPresent(renderer.get());
    }
}
