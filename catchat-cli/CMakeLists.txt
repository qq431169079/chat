add_executable(catchat-cli main.cpp)
find_package(Curses REQUIRED)

target_link_libraries(catchat-cli catchat ${CURSES_LIBRARIES})
target_include_directories(catchat-cli PRIVATE ${CURSES_INCLUDE_DIRS})
