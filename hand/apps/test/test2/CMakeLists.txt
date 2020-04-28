project(test2)

add_library(${PROJECT_NAME} SHARED test2.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBNAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${APPSDIR})
