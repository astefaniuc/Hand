target_sources(${LIBNAME} PRIVATE
    chord.cpp
    device.cpp
    hand.cpp
    inputstate.cpp
    node.cpp
    statetree.cpp)

add_subdirectory(sdl)
