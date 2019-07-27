#include <utility>
#include <iomanip>
#include <thread>
#include <chrono>

#include <SDL_FontCache.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <vic.h>
#include <Loop.h>
#include "cmrc/cmrc.hpp"

#include "sdl2.h"

#include "CIA1.h"
#include "CIA2.h"
#include "TimingConstants.h"
#include "CPU.h"
#include "StreamUtil.h"
#include "MMU.h"
#include "Clock.h"

CMRC_DECLARE(resources);

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

	auto window{sdl2::make_window("c64 Emulator", 0, 0, 403, 284, SDL_WINDOW_OPENGL)};
	auto renderer{sdl2::make_renderer(window.get(), -1, SDL_RENDERER_ACCELERATED)};

	auto texture
			{sdl2::make_bmp(renderer.get(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 403, 284)};

	std::string basicFileName = "rom/basic.rom";
	std::string kernalFileName = "rom/kernal.rom";
	std::string chargenFileName = "rom/chargen.rom";

	auto basicStream = fs.open(basicFileName);
	auto kernalStream = fs.open(kernalFileName);
	auto chargenStream = fs.open(chargenFileName);

	auto sans = sdl2::make_ttffont("res/font.ttf", 12);

	std::vector<u8> kernal{kernalStream.begin(), kernalStream.end()};
	std::vector<u8> basic{basicStream.begin(), basicStream.end()};
	std::vector<u8> chargen{chargenStream.begin(), chargenStream.end()};
	std::vector<u8> vicIO(0xffff);

	Clock clk{};
	SDL_Event event;
	CIA1 cia1{event};
	CIA2 cia2{};

	Screen screen{texture.get(), renderer.get()};
	ROM rom(kernal, basic, chargen, vicIO);
	MMU mmu(rom, cia1, cia2);
	CPU cpu(clk, mmu);
	mmu.write(1, 0x07u);
	VIC vic{clk, mmu, screen};
	Loop loop{screen, event};

	cia1.setGenerateInterrupt([&cpu]() {
		cpu.interruptRequest();
	});

	mmu.setVICWriteListener([&vic](const u16 &addr, const u8 &val) {
		return vic.set(addr, val);
	});

	mmu.setVICReadListener([&vic](const u16 &addr) -> u8 {
		return vic.get(addr);
	});
	using clock = std::chrono::high_resolution_clock;

	std::chrono::nanoseconds lag(0ns);
	auto time_start = clock::now();
	bool quit_game = false;

	game_state current_state;
	game_state previous_state;

	FC_Font *font = FC_CreateFont();
	FC_LoadFont(font, renderer.get(), "res/font.ttf", 20, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
	u64 buff{};
	bool dEnabled = false;
	cpu.setDebug(dEnabled);
	while (true) {
		if (!loop.update()) {
			break;
		}

		for (size_t amp = 0; amp < 10000; amp++) {
			buff++;
			if (amp % 100 == 0) {
				vic.tick();
			}
			if (amp % 5 == 0) {
				cia1.tick();
			}
			cpu.execute();
		}
		if (buff % TimingConstants::CONSOLE_RATIO == 0) {
			loop.draw();
		}

		loop.draw();
	}

	FC_FreeFont(font);

	SDL_Quit();

	return 0;
}
