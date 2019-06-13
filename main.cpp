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
#include "MMULoader.h"

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

int _main() {
	freopen("/tmp/outmy.txt", "w", stdout);
	std::vector<u8> vicIO(0xffff);
	Clock clk{}; //	Clock clk(std::chrono::milliseconds(1));
	ROM rom{};
	auto fs = cmrc::resources::get_filesystem();
	MMU mmu{rom};

	MMULoader mmuLoader{fs, mmu};
	mmuLoader.dumpToRAM("res/6502_functional_test.bin", 0x400);

	CPU cpu(clk, mmu, 0x400);

	cpu.write(1, 0);

	bool passed{};

	long long int cnt{};
	u16 pcPrev{};

	while (true) {
		cpu.execute();

		cnt++;

		if (cnt == 1000000) {
			std::cout << "progress\n";
			cnt = 0;
		}

		if (cpu.PC() == 0x3463) {
			passed = true;
			std::cout << std::hex << cpu.PC() << '\n';
			break;
		} else if (cpu.PC() == pcPrev) {
			std::cout << "loop\n";
			std::cout << std::hex << pcPrev << '\n';
			break;
		}

		pcPrev = cpu.PC();
	}

	std::cout << std::endl;
	for (size_t i = cpu.pcHist.size() - 5; i < cpu.pcHist.size(); i++) {
		std::cout << cpu.pcHist[i] << '\n';
	}
	if (passed) {
		std::cout << "Passed!";
	} else {
		std::cout << "Failed!";
	}

	return 0;
}

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
	std::vector<u8> vicIO(0xffff);

	SDL_Event event;

	Clock clk{}; //	Clock clk(std::chrono::milliseconds(1));
	ROM rom(kernal, basic, chargen, vicIO);
	MMU mmu(rom);
	CPU cpu(clk, mmu);


	auto t = std::thread([&cpu]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait until CPU is initialized.
		while (true) {
			cpu.setNMI(true);

			std::this_thread::sleep_for(std::chrono::milliseconds(9));
		}
	});

	unsigned buff{};
	try {
		while (true) {
			textRenderer.drawAndUpdate("asdasd");
			cpu.execute();
			char chars[] = {'!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
			++buff;
			if (buff == 1000000) {
				buff = 0;
				for (size_t i = 0; i < 25; i++) {
					for (size_t j = 0; j< 40; j++) {
						unsigned char c = mmu.read(0x400 + i * 25 + j);
						
						std::cout << (char) chars[(int) c - 0x21] << ' ';
//						std::cout << std::hex << std::uppercase << (int) c << ' ';
					}
					std::cout << '\n';
				}

				std::cout << '\n';
				std::cout << '\n';
			 }
		}
	} catch (const std::string &error) {
		std::cout << error << '\n';
	}

	t.join();
	SDL_Quit();

	return 0;
}
