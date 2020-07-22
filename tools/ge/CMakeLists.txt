add_executable(getool main.cpp)
target_link_libraries(getool PRIVATE ge)
set_target_properties(getool PROPERTIES OUTPUT_NAME "ge")
