#set includes
set(CMAKE_CXX_STANDARD 17)
include(${CMAKE_BINARY_DIR}/cmake-build-debug/conanbuildinfo.cmake)
conan_basic_setup()
include(CMakeRC.cmake)
target_link_libraries(NESEmuLib)

include_directories(${THIRD_PARTY_INCLUDE_PATH})
add_executable(NESTest ${source_files} catch.hpp CPUTest.cpp ../include/TimingConstants.h ../include/SDL_FontCache.h ../src/SDL_FontCache.cpp ../include/CIA1.h ../include/CIA2.h ../src/CIA1.cpp)


set(TEST_ASSET_COPY_DIR $<TARGET_FILE_DIR:NESTest>/res)
cmrc_add_resource_library(test_resources res/65C02_extended_opcodes_test.bin res/65C02_extended_opcodes_test.lst res/6502_functional_test.bin res/6502_functional_test.lst ALIAS test_resources::rc)
target_link_libraries(NESTest PRIVATE test_resources::rc ${CONAN_LIBS})


# Turn on CMake testing capabilities
enable_testing()

add_custom_command(
        TARGET NESTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${TEST_ASSET_COPY_DIR}
)