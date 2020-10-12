project(basicsdl1)

add_library(${PROJECT_NAME} SHARED
    basicsdl1.cpp
    drawersdl.cpp
    text.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBNAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${THEMESDIR})
