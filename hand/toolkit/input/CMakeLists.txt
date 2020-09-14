target_sources(${LIBNAME} PRIVATE
    device.cpp
    hand.cpp
    inputstate.cpp
    node.cpp
    statetree.cpp)

add_subdirectory(sdl)
