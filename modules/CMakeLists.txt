set(GE_MODULES
    error
    math
    renderer
    time
    ui
)

foreach(module ${GE_MODULES})
    add_subdirectory(${module})
endforeach()

add_library(ge dummy.cpp)
target_include_directories(ge PUBLIC ${PROJECT_SOURCE_DIR}/include)
foreach(module ${GE_MODULES})
    target_link_libraries(ge PUBLIC ${module})
endforeach()
add_dependencies(ge ${GE_MODULES})
