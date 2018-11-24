cmake_minimum_required(VERSION 3.10)
project(NESEmu)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)

file(GLOB source_files
        "include/*.h"
        "src/*.cpp")

#set library
add_library (NESEmuLib STATIC ${source_files})

include_directories(include)

add_executable(NESEmu ${source_files} main.cpp "test.nes")

target_link_libraries(NESEmu ${CONAN_LIBS})

add_subdirectory(test)

