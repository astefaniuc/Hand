target_sources(${LIBNAME} PRIVATE
    search.cpp
    searchcookie.cpp)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
