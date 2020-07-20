project(default)

add_library(${PROJECT_NAME} SHARED 
    default.cpp 
    drawers.cpp
    virtualsurface.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBNAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${THEMESDIR})
