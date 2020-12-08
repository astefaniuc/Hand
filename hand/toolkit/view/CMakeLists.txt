add_subdirectory(datatypes)
add_subdirectory(layers)
add_subdirectory(layouts)

target_sources(${LIBNAME} PRIVATE
    field.cpp
    layer.cpp
    layout.cpp)
