#include <utility>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <tuple>
#include <thread>
#include <iostream>
#include <chrono>
#include <iomanip>

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "cmrc/cmrc.hpp"
#include "boost/filesystem.hpp"

#include "sdl2.h"

#include "MMULoader.h"
#include "TimingConstants.h"
#include "CPU.h"
#include "StreamUtil.h"
#include "GUI.h"
#include "MMU.h"
#include "Clock.h"

CMRC_DECLARE(resources);

class TextRenderer {
	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Color *color;
public:
	TextRenderer(SDL_Renderer *renderer, TTF_Font *font, SDL_Color *color)
			: renderer(renderer),
				font(font),
				color(color) {}

	void drawAndUpdate(const std::string &text) {
		auto surface = sdl2::make_font(font, text);
		auto texture = sdl2::make_texture(renderer, surface.get());
		int texW{}, texH{};
		SDL_QueryTexture(texture.get(), nullptr, nullptr, &texW, &texH);
		SDL_Rect dstrect{0, texH, texW, texH};
		SDL_RenderCopy(renderer, texture.get(), nullptr, &dstrect);
//		SDL_RenderPresent(renderer);
	}

	void drawAndUpdate(const std::string &text, int line) {
		auto surface = sdl2::make_font(font, text);
		auto texture = sdl2::make_texture(renderer, surface.get());
		int texW{}, texH{};
		SDL_QueryTexture(texture.get(), nullptr, nullptr, &texW, &texH);
		SDL_Rect dstrect{0, line * texH, texW, texH};
		SDL_RenderCopy(renderer, texture.get(), nullptr, &dstrect);
//		SDL_RenderPresent(renderer);
	}
};

#include <chrono>
#include <SDL_FontCache.h>

using namespace std::chrono_literals;

// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
constexpr std::chrono::nanoseconds timestep(16ms);

struct game_state {
	// this contains the state of your game, such as positions and velocities
};

bool handle_events() {
	// poll for events

	return false; // true if the user wants to quit the game
}

void update(game_state *) {
	// update game logic here
}

void render(game_state const &) {
	// render stuff here
}

game_state interpolate(game_state const &current, game_state const &previous, float alpha) {
	game_state interpolated_state;

	// interpolate between previous and current by alpha here

	return interpolated_state;
}

int main(int argc, char *args[]) {
	auto fs = cmrc::resources::get_filesystem();
	std::ios_base::sync_with_stdio(false);
	sdl2::TTFContext ttfContext{};
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
	}

	constexpr u16 WINDOW_WIDTH = 40 * 10, WINDOW_HEIGHT = 25 * 10;
	auto window{sdl2::make_window("c64 Emulator", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL)};
	auto renderer{sdl2::make_renderer(window.get(), -1, SDL_RENDERER_ACCELERATED)};
	auto texture
			{sdl2::make_bmp(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT)};

	std::string basicFileName = "rom/basic.rom";
	std::string kernalFileName = "rom/kernal.rom";
	std::string chargenFileName = "rom/chargen.rom";

	auto basicStream = fs.open(basicFileName);
	auto kernalStream = fs.open(kernalFileName);
	auto chargenStream = fs.open(chargenFileName);

	auto sans = sdl2::make_ttffont("res/font.ttf", 12);

	TextRenderer textRenderer{renderer.get(), sans.get(), new SDL_Color()};

	std::vector<u8> kernal{kernalStream.begin(), kernalStream.end()};
	std::vector<u8> basic{basicStream.begin(), basicStream.end()};
	std::vector<u8> chargen{chargenStream.begin(), chargenStream.end()};
	std::vector<u8> vicIO(0xffff);

	Clock clk{};
//	Clock clk(std::chrono::microseconds(1));
	ROM rom(kernal, basic, chargen, vicIO);
	MMU mmu(rom);
	CPU cpu(clk, mmu);

	const char chars[] =
			{'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U',
			 'V', 'W', 'X', 'Y', 'Z',
			 '[', ' ', ']', ' ', ' ', ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0',
			 '1', '2', '3', '4', '5',
			 '6', '7', '8', '9', ':', ';', '<', '=', '>', '?'};


	using clock = std::chrono::high_resolution_clock;

	std::chrono::nanoseconds lag(0ns);
	auto time_start = clock::now();
	bool quit_game = false;

	game_state current_state;
	game_state previous_state;

	FC_Font *font = FC_CreateFont();
	FC_LoadFont(font, renderer.get(), "res/font.ttf", 20, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
	std::chrono::high_resolution_clock::time_point start(
			std::chrono::high_resolution_clock::now());
	u64 buff{};
	bool initial = true;
	while (!quit_game) {
		// NOTE: This must be called!
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				break;
			}
		}

		for (size_t amp = 0; amp < 10000; amp++) {
			buff++;
			auto milliseconds =
					std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

			if (initial && milliseconds >= std::chrono::seconds(1)) {
				initial = false;
			}

			if (!initial && milliseconds >= std::chrono::milliseconds(200)) {
				cpu.setNMI(true);
				start = std::chrono::high_resolution_clock::now();
			}

			cpu.execute();

			if (buff % TimingConstants::CONSOLE_RATIO == 0) {
				for (size_t i = 0; i < 25; i++) {
					std::string lineBuff(40, ' ');
					for (size_t j = 0; j < 40; j++) {
						unsigned char c = mmu.read(0x400 + i * 25 + j + i * 15);
						if (c < 64 && chars[c] != ' ') {
							lineBuff[j] = chars[c];
						}
					}
					textRenderer.drawAndUpdate(lineBuff, i);
				}
			}
		}

		std::cout << '\n';
		std::cout << '\n';

		auto delta_time = clock::now() - time_start;
		time_start = clock::now();
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

		quit_game = handle_events();

		// update game logic as lag permits
		while (lag >= timestep) {
			lag -= timestep;

			previous_state = current_state;
			update(&current_state); // update at a fixed rate each time
		}

		// calculate how close or far we are from the next timestep
		auto alpha = (float) lag.count() / timestep.count();
		auto interpolated_state = interpolate(current_state, previous_state, alpha);

//		render(interpolated_state);
//		FC_Draw(font, renderer.get(), 0, 0, "This is %s.\n It works.", "example text");


		SDL_RenderPresent(renderer.get());
		SDL_RenderClear(renderer.get());
	}

	FC_FreeFont(font);

//	t.join();
	SDL_Quit();

	return 0;
}
/*
int _main(int argc, char *args[]) {
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
	std::vector<u8> vicIO(0xffff);

	SDL_Event event;

	Clock clk{};
//	Clock clk(std::chrono::microseconds(1));
	ROM rom(kernal, basic, chargen, vicIO);
	MMU mmu(rom);
	CPU cpu(clk, mmu);


//	auto clockThread = std::thread([&clk]() {
//		clk.startTicking();
//	});
//
	auto t = std::thread([&cpu]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait until CPU is initialized.
		while (true) {
//			cpu.setNMI(true);
//
//			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	});

	const char chars[] =
			{'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U',
			 'V', 'W', 'X', 'Y', 'Z',
			 '[', ' ', ']', ' ', ' ', ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0',
			 '1', '2', '3', '4', '5',
			 '6', '7', '8', '9', ':', ';', '<', '=', '>', '?'};

	std::chrono::high_resolution_clock::time_point start(
			std::chrono::high_resolution_clock::now());
	u64 buff{};
	bool initial = true;
	try {
		while (true) {
			buff++;
			auto milliseconds =
					std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

			if (initial && milliseconds >= std::chrono::seconds(1)) {
				initial = false;
			}

			if (!initial && milliseconds >= std::chrono::milliseconds(200)) {
				cpu.setNMI(true);
				start = std::chrono::high_resolution_clock::now();
			}

			if (buff % TimingConstants::PAL_CPU_RATIO == 0) {
				cpu.execute();
			}

			if (buff % TimingConstants::CONSOLE_RATIO == 0) {
				for (size_t i = 0; i < 25; i++) {
					for (size_t j = 0; j < 40; j++) {
						unsigned char c = mmu.read(0x400 + i * 25 + j + i * 15);
//						std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int) c << ' ';
						if (c < 64 && chars[c] != ' ') {
							std::cout << (char) chars[c];
						} else {
							std::cout << '_';
						}
					}
					std::cout << '\n';
				}

				std::cout << '\n';
				std::cout << '\n';
			}

			if (buff % TimingConstants::PAL_KEYBOARD_CYCLES == 0) {
				cpu.interruptRequest();
				cpu.setNMI(false);
				cpu.execute();
			}
//			SDL_Delay(3000);
//			std::this_thread::sleep_for(std::chrono::nanoseconds(TimingConstants::PAL_MASTER_NANOS));
		}
	} catch (const std::string &error) {
		std::cout << error << '\n';
	}

//	t.join();
	SDL_Quit();

	return 0;
}

 */
