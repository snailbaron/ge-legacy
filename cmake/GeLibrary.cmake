include(GeMessages)

macro(ge_library)
    set(options HEADER_ONLY)
    set(one_value_args "")
    set(multi_value_args DEPENDS FLATBUFFERS SOURCES)
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

    if(GE_LIBRARY_FLATBUFFERS)
        foreach(path ${GE_LIBRARY_FLATBUFFERS})
            set(full_path "${CMAKE_CURRENT_SOURCE_DIR}/${path}")

            get_filename_component(name_base ${path} NAME_WLE)
            set(gen_name "${name_base}_generated.h")

            add_custom_command(
                COMMENT "generate header from ${full_path}"
                OUTPUT
                    "${CMAKE_CURRENT_BINARY_DIR}/generated/${gen_name}"
                    "${CMAKE_CURRENT_SOURCE_DIR}/${gen_name}"
                COMMAND
                    $<TARGET_FILE:flatc>
                        --cpp
                        -o "${CMAKE_CURRENT_BINARY_DIR}/generated/"
                        ${full_path}
                COMMAND
                    ${CMAKE_COMMAND} -E create_symlink
                        "${CMAKE_CURRENT_BINARY_DIR}/generated/${gen_name}"
                        "${CMAKE_CURRENT_SOURCE_DIR}/${gen_name}"
                DEPENDS ${full_path}
            )
            add_custom_target(
                "flatc_${name_base}"
                DEPENDS
                    "${CMAKE_CURRENT_BINARY_DIR}/generated/${gen_name}"
                    "${CMAKE_CURRENT_SOURCE_DIR}/${gen_name}"
            )
            add_dependencies(${GE_LIBRARY_NAME} "flatc_${name_base}")
        endforeach()
    endif()
endmacro()
