#include <utility>

#include <SDL2/SDL.h>
#include "cmrc/cmrc.hpp"

#include "sdl2.h"
#include "CIA1.h"
#include "vic.h"
#include "Loop.h"
#include "PRGLoader.h"
#include "Keyboard.h"
#include "CIA2.h"
#include "TimingConstants.h"
#include "CPU.h"
#include "StreamUtil.h"
#include "MMU.h"
#include "Clock.h"

CMRC_DECLARE(resources);

using namespace std::chrono_literals;

int runCustomROM(const std::string &customROMPath) {
    auto fs = cmrc::resources::get_filesystem();
    std::ios_base::sync_with_stdio(false);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }

    auto window{
            sdl2::make_window("c64 Emulator", 0, 0, GraphicsConstants::WINDOW_WIDTH, GraphicsConstants::WINDOW_HEIGHT,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)};
    auto renderer{sdl2::make_renderer(window.get(), -1, SDL_RENDERER_ACCELERATED)};
    auto texture
            {sdl2::make_bmp(renderer.get(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
                            GraphicsConstants::WINDOW_WIDTH, GraphicsConstants::WINDOW_HEIGHT)};

    Screen screen{texture.get(), renderer.get()};
    SDL_Event event;
    Loop loop{screen, event};

    auto is = std::ifstream(customROMPath, std::ios::in | std::ios::binary);
    is.unsetf(std::ios::skipws);

    auto customROMStream = StreamUtil::toVectorU8(is);
    auto basicStream = fs.open("rom/basic.rom");
    auto kernalStream = fs.open("rom/kernal.rom");
    auto chargenStream = fs.open("rom/chargen.rom");

    std::vector<u8> kernal{kernalStream.begin(), kernalStream.end()};
    std::vector<u8> basic{basicStream.begin(), basicStream.end()};
    std::vector<u8> chargen{chargenStream.begin(), chargenStream.end()};
    std::vector<u8> customROM{customROMStream.begin(), customROMStream.end()};

    Clock clk{};
    CIA1 cia1{event};
    CIA2 cia2{};

    std::vector<u8> vicIO(0xffff);
    ROM rom{kernal, basic, chargen, vicIO, customROM};
    MMU mmu{rom, cia1, cia2};
    CPU cpu{clk, mmu};
    VIC vic{clk, mmu, screen};

    cia1.setGenerateInterrupt([&cpu]() {
        cpu.interruptRequest();
    });

    mmu.setVICWriteListener([&vic](const u16 &addr, const u8 &val) {
        return vic.set(addr, val);
    });

    mmu.setVICReadListener([&vic](const u16 &addr) -> u8 {
        return vic.get(addr);
    });

    u64 buff{};
    bool dEnabled = false;
    cpu.setDebug(dEnabled);
    Keyboard keyboard;
    while (true) {
        if (!loop.update()) {
            break;
        }

        for (size_t amp = 0; amp < 10000; amp++) {
            buff++;
            if (amp % 63 == 0) {
                vic.tick();
            }
            if (amp % 5 == 0) {
                cia1.tick();
            }
            cpu.execute();
        }

        if (keyboard.isKeyPressed((SDL_SCANCODE_F5))) {
            PRGLoader::loadPRG(mmu, rom);
        }
        if (buff % TimingConstants::CONSOLE_RATIO == 0) {
            loop.draw();
        }

        loop.draw();
    }

    SDL_Quit();

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        runCustomROM(argv[1]);
    }
    return 0;
}

