set(GE_LIBS
    client
    error
    math
    resources
    str
    time
    tyke
    ui
    util
)

foreach(ge_lib ${GE_LIBS})
    add_subdirectory(${ge_lib})
endforeach()

add_library(ge dummy.cpp)
target_include_directories(ge PUBLIC ${PROJECT_SOURCE_DIR}/include)
foreach(ge_lib ${GE_LIBS})
    target_link_libraries(ge PUBLIC ${ge_lib})
endforeach()
#add_dependencies(ge ${GE_LIBS})
