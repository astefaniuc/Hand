add_subdirectory(datatypes)
add_subdirectory(layer)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
