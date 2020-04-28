project(test1)

add_library(${PROJECT_NAME} SHARED test1.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBNAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${APPSDIR})
