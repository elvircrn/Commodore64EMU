#set includes
target_link_libraries(NESEmuLib)
include_directories (${THIRD_PARTY_INCLUDE_PATH})
add_executable (NESTest ${source_files} catch.hpp CPUTest.cpp)


# SDL2
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(NESTest ${SDL2_LIBRARIES})


# Boost
set(boost_min_ver 1.50.0)
set(boost_libs system filesystem)
find_package(Boost ${boost_min_ver})
if (Boost_FOUND)
    find_package(Boost ${boost_min_ver} COMPONENTS ${boost_libs})
endif ()
target_link_libraries(NESTest ${Boost_LIBRARIES})


# Turn on CMake testing capabilities
enable_testing()
