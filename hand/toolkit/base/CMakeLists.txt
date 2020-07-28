target_sources(${LIBNAME} PRIVATE
    filesystem.cpp
    modulelib.cpp
    handserver.cpp)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
