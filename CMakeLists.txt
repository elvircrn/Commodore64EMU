cmake_minimum_required(VERSION 3.10)
project(NESEmu)

set(CMAKE_CXX_STANDARD 17)


include_directories(.)
include_directories(include)
include_directories(include/SDL2)
include_directories("NES Emulator")
include_directories(NESEmulatorTest)


add_executable(NESEmu
        "NES Emulator/core.cpp"
        "NES Emulator/core.h"
        "NES Emulator/CPU.cpp"
        "NES Emulator/CPU.h"
        "NES Emulator/Debugger.cpp"
        "NES Emulator/Debugger.h"
        "NES Emulator/donkeykong.nes"
        "NES Emulator/EmuException.cpp"
        "NES Emulator/EmuException.h"
        "NES Emulator/FileHandler.cpp"
        "NES Emulator/FileHandler.h"
        "NES Emulator/GUI.cpp"
        "NES Emulator/GUI.h"
        "NES Emulator/Instructions.cpp"
        "NES Emulator/Instructions.h"
        "NES Emulator/main.cpp"
        "NES Emulator/NES Emulator.vcxproj"
        "NES Emulator/NES Emulator.vcxproj.filters"
        "NES Emulator/NES.cpp"
        "NES Emulator/NES.h"
        "NES Emulator/nestest.nes"
        "NES Emulator/Palette.cpp"
        "NES Emulator/Palette.h"
        "NES Emulator/PPU.cpp"
        "NES Emulator/PPU.h"
        "NES Emulator/ROM.cpp"
        "NES Emulator/ROM.h"
        "NES Emulator/SDLCore.cpp"
        "NES Emulator/SDLCore.h"
        "NES Emulator/test.nes"
        NESEmulatorTest/CPUTest.cpp
        NESEmulatorTest/NESEmulatorTest.vcxproj
        NESEmulatorTest/NESEmulatorTest.vcxproj.filters
        NESEmulatorTest/nestest.nes
        NESEmulatorTest/nestest.txt
        NESEmulatorTest/res.txt
        NESEmulatorTest/timing.txt
        6502.jpg
        NESEmulator.sln
        README.md
        timings.txt "NES Emulator/optional.h" "NES Emulator/variant.h" "NES Emulator/catch.hpp")
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(NESEmu ${SDL2_LIBRARIES})


set(boost_min_ver 1.50.0)
set(boost_libs system filesystem)
find_package(Boost ${boost_min_ver})

if(Boost_FOUND)
    find_package(Boost ${boost_min_ver} COMPONENTS ${boost_libs})
endif()




target_link_libraries(NESEmu ${Boost_LIBRARIES})

enable_testing()