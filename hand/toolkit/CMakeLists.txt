cmake_minimum_required(VERSION 3.13)
project(${LIBNAME})


add_library(${LIBNAME} SHARED "")

target_compile_definitions(${LIBNAME} PRIVATE 
    HAND_APPSDIR="${APPSDIR}"
    HAND_THEMESDIR="${THEMESDIR}")

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_subdirectory(base)
add_subdirectory(graph)
add_subdirectory(input)
add_subdirectory(view)

target_link_libraries(${LIBNAME} PUBLIC dl boost_filesystem boost_system SDL SDL_ttf)


install(TARGETS ${LIBNAME} DESTINATION ${LIBDIR})
