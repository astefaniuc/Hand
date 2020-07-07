target_sources(${LIBNAME} PRIVATE
    device.cpp
    hand.cpp
    inputstate.cpp
    keyboard.cpp
    node.cpp
    statetree.cpp)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
