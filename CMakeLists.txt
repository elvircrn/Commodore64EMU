cmake_minimum_required(VERSION 3.10)
project(NESEmu)

set(CMAKE_CXX_STANDARD 17)

file(GLOB source_files
        "include/*.h"
        "src/*.cpp")

#set library
add_library (NESEmuLib STATIC ${source_files})

include_directories(include)

add_executable(NESEmu ${source_files} main.cpp "test.nes" 6502.jpg README.md timings.txt)

# SDL2
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(NESEmu ${SDL2_LIBRARIES})

# Boost
set(boost_min_ver 1.50.0)
set(boost_libs system filesystem)
find_package(Boost ${boost_min_ver})
if (Boost_FOUND)
    find_package(Boost ${boost_min_ver} COMPONENTS ${boost_libs})
endif ()
target_link_libraries(NESEmu ${Boost_LIBRARIES})

add_subdirectory(test)

