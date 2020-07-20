target_sources(${LIBNAME} PRIVATE
    buttonlayer.cpp
    layer.cpp
    layermanager.cpp
    listlayer.cpp
    textlayer.cpp
    user.cpp)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
