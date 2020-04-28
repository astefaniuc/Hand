cmake_minimum_required(VERSION 3.13)
project(Hand)


set(LIBNAME toolkit)
set(LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
set(APPSDIR ${LIBDIR}/apps/)
set(THEMESDIR ${LIBDIR}/themes/)



add_subdirectory(toolkit)
add_subdirectory(apps)

add_executable(hand hand.cpp)
target_link_libraries(hand toolkit)


install(TARGETS hand)
