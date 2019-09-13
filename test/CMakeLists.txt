#set includes
set(CMAKE_CXX_STANDARD 17)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
include(CMakeRC.cmake)
target_link_libraries(C64EmuLib)

include_directories(${THIRD_PARTY_INCLUDE_PATH})
add_executable(C64Test ${source_files} catch.hpp CPUTest.cpp BitTest.cpp)


set(TEST_ASSET_COPY_DIR $<TARGET_FILE_DIR:C64Test>/res)
cmrc_add_resource_library(test_resources res/65C02_extended_opcodes_test.bin res/65C02_extended_opcodes_test.lst res/6502_functional_test.bin res/6502_functional_test.lst ALIAS test_resources::rc)
target_link_libraries(C64Test PRIVATE test_resources::rc ${CONAN_LIBS})


# Turn on CMake testing capabilities
enable_testing()

add_custom_command(
        TARGET C64Test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${TEST_ASSET_COPY_DIR}
)