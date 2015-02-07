add_library(catchat dht.c catchat.cpp dht_cmd.cpp keygen.cpp)

find_package(Botan REQUIRED)
find_package(LibConfig++ REQUIRED)
find_package(LibEV REQUIRED)

target_include_directories(catchat
                            PUBLIC
                                ${CMAKE_CURRENT_SOURCE_DIR}
                            PRIVATE
                                ${BOTAN_INCLUDE_DIR}
                                ${LIBCONFIG_INCLUDE_DIR}
                                ${LIBEV_INCLUDE_DIR} )

target_link_libraries(catchat
                        ${BOTAN_LIBRARIES}
                        ${LIBCONFIG_LIBRARIES}
                        ${LIBCONFIGPP_LIBRARIES}
                        ${LIBEV_LIBRARY})
