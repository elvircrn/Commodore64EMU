#set includes
set(CMAKE_CXX_STANDARD 17)
include(${CMAKE_BINARY_DIR}/cmake-build-debug/conanbuildinfo.cmake)
conan_basic_setup()
target_link_libraries(NESEmuLib)

include_directories (${THIRD_PARTY_INCLUDE_PATH})
add_executable (NESTest ${source_files} catch.hpp CPUTest.cpp)


target_link_libraries(NESTest ${CONAN_LIBS})


# Turn on CMake testing capabilities
enable_testing()
