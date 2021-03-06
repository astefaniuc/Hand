cmake_minimum_required(VERSION 3.13)
project(Hand)


set(LIBNAME toolkit)
set(LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
set(APPSDIR ${LIBDIR}/apps/)
set(THEMESDIR ${LIBDIR}/themes/)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

add_subdirectory(toolkit)
add_subdirectory(apps)

add_executable(hand main.cpp)
target_link_libraries(hand toolkit)


install(TARGETS hand)
