add_library(catchat dht.c catchat.cpp)

find_package(Botan REQUIRED)

target_include_directories(catchat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE ${BOTAN_INCLUDE_DIR})

target_link_libraries(catchat ${BOTAN_LIBRARIES})
