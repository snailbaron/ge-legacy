macro(ge_program)
    set(options "")
    set(one_value_args NAME)
    set(multi_value_args SOURCES)
    cmake_parse_arguments(
        GE_PROGRAM
            "${options}"
            "${one_value_args}"
            "${multi_value_args}"
            ${ARGN})

    add_executable(${GE_PROGRAM_NAME} ${GE_PROGRAM_SOURCES})
    target_link_libraries(${GE_PROGRAM_NAME} PRIVATE ge)
endmacro()
