add_subdirectory(datatypes)
add_subdirectory(layer)

target_sources(${LIBNAME} PRIVATE layout.cpp)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
