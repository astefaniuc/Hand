project(default)

add_library(${PROJECT_NAME} SHARED default.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBNAME})
