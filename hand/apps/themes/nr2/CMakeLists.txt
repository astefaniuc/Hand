project(nr2)

add_library(${PROJECT_NAME} SHARED nr2.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBNAME})
