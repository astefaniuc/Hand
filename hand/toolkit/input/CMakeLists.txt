target_sources(${LIBNAME} PRIVATE
    device.cpp
    inputstate.cpp
    node.cpp
    statetree.cpp)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
