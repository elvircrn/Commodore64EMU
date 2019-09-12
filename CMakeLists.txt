cmake_minimum_required(VERSION 3.10)
project(NESEmu)

include(CMakeRC.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)

set(ASSET_COPY_DIR $<TARGET_FILE_DIR:NESEmu>/res)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O3 -flto -fno-inline")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O3 -flto")

file(GLOB source_files
        "include/*.h"
        "src/*.cpp")

#set library
add_library(NESEmuLib STATIC ${source_files})
include_directories(include)

add_executable(NESEmu ${source_files} main.cpp)

cmrc_add_resource_library(resources res/6502_functional_test.bin rom/kernal.rom rom/basic.rom rom/chargen.rom ALIAS resources::rc)
target_link_libraries(NESEmu PRIVATE resources::rc ${CONAN_LIBS})

add_subdirectory(test)

add_custom_command(
        TARGET NESEmu POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${ASSET_COPY_DIR}
)
