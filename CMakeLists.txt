cmake_minimum_required(VERSION 3.10)
project(NESEmu)

include(${CMAKE_BINARY_DIR}/cmake-build-debug/conanbuildinfo.cmake)
conan_basic_setup()
include(CMakeRC.cmake)

set(CMAKE_CXX_STANDARD 17)

set(ASSET_COPY_DIR $<TARGET_FILE_DIR:NESEmu>/res)


file(GLOB source_files
        "include/*.h"
        "src/*.cpp")

#set library
add_library(NESEmuLib STATIC ${source_files})


include_directories(include)

add_executable(NESEmu ${source_files} main.cpp "rom/basic.rom" "rom/kernal.rom" "rom/chargen.rom" "res/font.ttf" src/vic.cpp)

cmrc_add_resource_library(resources rom/kernal.rom rom/basic.rom rom/chargen.rom "res/font.ttf" ALIAS resources::rc)
target_link_libraries(NESEmu PRIVATE resources::rc ${CONAN_LIBS})

add_subdirectory(test)

add_custom_command(
        TARGET NESEmu POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${ASSET_COPY_DIR}
)
