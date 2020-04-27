cmake_minimum_required(VERSION 3.13)
project(Hand)


set(LIBNAME toolkit)

add_subdirectory(toolkit)
add_subdirectory(apps)

add_executable(hand hand.cpp)
target_link_libraries(hand toolkit)
