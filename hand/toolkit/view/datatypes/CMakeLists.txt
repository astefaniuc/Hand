target_sources(${LIBNAME} PRIVATE
    rect.cpp
    rgb.cpp)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
