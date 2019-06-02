#include <utility>

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <chrono>
#include "cmrc/cmrc.hpp"

#include "boost/filesystem.hpp"
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "CPU.h"
#include "StreamUtil.h"
#include "GUI.h"
#include "MMU.h"
#include "Clock.h"
#include "sdl2.h"

CMRC_DECLARE(resources);

class TextRenderer {
	sdl2::renderer_sh_ptr_t renderer;
	sdl2::ttffont_sh_ptr_t font;
	SDL_Color color;
public:
	TextRenderer(sdl2::renderer_sh_ptr_t renderer, sdl2::ttffont_sh_ptr_t font, const SDL_Color &color = {255, 255, 255})
			: renderer(std::move(renderer)),
				font(std::move(font)),
				color(color) {}

	void drawAndUpdate(const std::string &text) {
		auto surface = sdl2::make_font(font.get(), text, color);
		auto texture = sdl2::make_texture(renderer.get(),
																			surface.get());
		int texW{}, texH{};
		SDL_QueryTexture(texture.get(), nullptr, nullptr, &texW, &texH);
		SDL_Rect dstrect{0, 0, texW, texH};
		SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &dstrect);
		SDL_RenderPresent(renderer.get());
	}
};

int main(int argc, char *args[]) {
	auto fs = cmrc::resources::get_filesystem();
	std::ios_base::sync_with_stdio(false);
	sdl2::TTFContext ttfContext{};
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
	}

	constexpr u16 WINDOW_WIDTH = 320, WINDOW_HEIGHT = 200;
	auto window = sdl2::make_window("c64 Emulator", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	auto renderer = sdl2::make_renderer(window.get(), -1, 0);
	auto texture =
			sdl2::make_bmp(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

	std::string basicFileName = "rom/basic.rom";
	std::string kernalFileName = "rom/kernal.rom";
	std::string chargenFileName = "rom/chargen.rom";

	auto basicStream = fs.open(basicFileName);
	auto kernalStream = fs.open(kernalFileName);
	auto chargenStream = fs.open(chargenFileName);

	auto sans = sdl2::make_ttffont("res/font.ttf", 12);

	TextRenderer textRenderer{std::move(renderer), std::move(sans)};

	std::vector<u8> kernal{kernalStream.begin(), kernalStream.end()};
	std::vector<u8> basic{basicStream.begin(), basicStream.end()};
	std::vector<u8> chargen{chargenStream.begin(), chargenStream.end()};

	SDL_Event event;

	Clock clk{}; //	Clock clk(std::chrono::milliseconds(1));
	ROM rom(kernal, basic, chargen);
	IO io(event);
	MMU mmu(io, rom);
	CPU cpu(clk, mmu);


	short buff{};
	try {
		while (true) {
			++buff;
			cpu.execute();

			if (buff == 0) {
				std::string str{};
				for (size_t i = 0; i < 30; i++) {
					unsigned char c = mmu.read(0x0400 + i);
					std::cout << (int) c << ' ';
					str.push_back(c);
				}
				std::cout << '\n';
			}
		}
	} catch (const std::string &error) {
		std::cout << error << '\n';
	}

	SDL_Quit();

	return 0;
}
