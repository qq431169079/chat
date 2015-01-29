cmake_minimum_required(VERSION 2.8.12)
project(catchat)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if(BUILD_SHARED_LIBS)
    add_definitions(-DCATCHAT_SHARED)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -Wall -Wextra -pedantic -Werror -std=c11")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -Wextra -pedantic -Werror -std=c++11")
endif()


add_subdirectory(libcatchat)
add_subdirectory(catchat-cli)
