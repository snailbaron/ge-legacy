include(GeMessages)

macro(ge_library)
    set(options HEADER_ONLY)
    set(one_value_args "")
    set(multi_value_args DEPENDS SOURCES)
    cmake_parse_arguments(
        GE_LIBRARY
            "${options}"
            "${one_value_args}"
            "${multi_value_args}"
            ${ARGN})

    get_filename_component(GE_LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    if(GE_LIBRARY_UNPARSED_ARGUMENTS)
        ge_error("'${GE_LIBRARY_NAME}' library: unknown arguments: ${GE_LIBRARY_UNPARSED_ARGUMENTS}")
    endif()

    if(GE_LIBRARY_KEYWORDS_MISSING_VALUES)
        ge_error("'${GE_LIBRARY_NAME}' library: missing values: ${GE_LIBRARY_KEYWORDS_MISSING_VALUES}")
    endif()

    if(GE_LIBRARY_HEADER_ONLY)
        add_library(${GE_LIBRARY_NAME} INTERFACE)
        target_include_directories(
            ${GE_LIBRARY_NAME} INTERFACE include)

        if(GE_LIBRARY_DEPENDS)
            foreach(dep ${GE_LIBRARY_DEPENDS})
                target_link_libraries(${GE_LIBRARY_NAME} INTERFACE ${dep})
            endforeach()
        endif()
    elseif(GE_LIBRARY_SOURCES)
        add_library(${GE_LIBRARY_NAME} ${GE_LIBRARY_SOURCES})
        target_include_directories(${GE_LIBRARY_NAME} PUBLIC include)

        if(GE_LIBRARY_DEPENDS)
            foreach(dep ${GE_LIBRARY_DEPENDS})
                target_link_libraries(${GE_LIBRARY_NAME} PUBLIC ${dep})
            endforeach()
        endif()
    else()
        ge_error("'${GE_LIBRARY_NAME}' library must either have SOURCES or be HEADER_ONLY")
    endif()

endmacro()
