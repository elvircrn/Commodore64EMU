#include <system_error>

#include "SDLCore.h"

template<typename Creator, typename Destructor, typename... Arguments>
auto sdl2::make_resource(Creator c, Destructor d, Arguments &&... args) {
	auto r = c(std::forward<Arguments>(args)...);
	if (!r) {
		throw std::system_error(errno, std::generic_category(), SDL_GetError());
	}
	return std::unique_ptr<std::decay_t<decltype(*r)>, decltype(d)>(r, d);
}

sdl2::window_ptr_t sdl2::make_window(const char *title,
																		 int x, int y, int w, int h, Uint32 flags) {
	return make_resource(SDL_CreateWindow, SDL_DestroyWindow,
											 title, x, y, w, h, flags);
}

sdl2::renderer_ptr_t sdl2::make_renderer(const sdl2::window_ptr_t &ptr, int index, Uint32 flags) {
	return make_resource(SDL_CreateRenderer, SDL_DestroyRenderer, ptr.get(), index, flags);
}

sdl2::texture_ptr_t sdl2::make_texture(const sdl2::renderer_ptr_t &renderer, Uint32 format, int access, int w, int h) {
	return make_resource(SDL_CreateTexture, SDL_DestroyTexture, renderer.get(), format, access, w, h);
}
