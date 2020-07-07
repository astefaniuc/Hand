target_sources(${LIBNAME} PRIVATE
    collection.cpp
    data.cpp
    interface.cpp
    vertex.cpp)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
