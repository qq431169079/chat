add_library(catchat dht.c catchat.cpp)

find_package(Botan REQUIRED)

target_include_directories(catchat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
