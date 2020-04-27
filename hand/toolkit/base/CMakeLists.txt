target_sources(${LIBNAME} PRIVATE
    datamanager.cpp
    factory.cpp
    filesystem.cpp
    handapp.cpp
    handserver.cpp)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
