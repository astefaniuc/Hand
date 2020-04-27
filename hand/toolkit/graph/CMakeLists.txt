add_subdirectory(search)

target_sources(${LIBNAME} PRIVATE
    link.cpp
    list.cpp
    vertex.cpp)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
