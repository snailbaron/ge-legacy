include(GeMessages)

macro(ge_target)
    set(options HEADER_ONLY PROGRAM)
    set(one_value_args NAME)
    set(multi_value_args DEPENDS FLATBUFFERS RESOURCES SOURCES)
    cmake_parse_arguments(
        GE_TARGET
            "${options}"
            "${one_value_args}"
            "${multi_value_args}"
            ${ARGN})

    if(NOT DEFINED GE_TARGET_NAME)
        get_filename_component(GE_TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    endif()

    if(TARGET ${GE_TARGET_NAME})
        get_target_property(existing_target_full_path ${GE_TARGET_NAME} SOURCE_DIR)
        file(RELATIVE_PATH existing_target_relative_path ${CMAKE_SOURCE_DIR} ${existing_target_full_path})
        file(RELATIVE_PATH current_relative_path ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
        ge_error("ge target '${GE_TARGET_NAME}': cannot create at '${current_relative_path}', already exists at ${existing_target_relative_path}")
    endif()
    if(GE_TARGET_UNPARSED_ARGUMENTS)
        ge_error("ge target '${GE_TARGET_NAME}': unknown arguments: ${GE_TARGET_UNPARSED_ARGUMENTS}")
    endif()
    if(GE_TARGET_KEYWORDS_MISSING_VALUES)
        ge_error("ge target '${GE_TARGET_NAME}': missing values: ${GE_TARGET_KEYWORDS_MISSING_VALUES}")
    endif()
    if(NOT GE_TARGET_SOURCES AND NOT GE_TARGET_HEADER_ONLY)
        ge_error("ge target '${GE_TARGET_NAME}': must either have SOURCES or be HEADER_ONLY")
    endif()
    if(GE_TARGET_SOURCES AND GE_TARGET_HEADER_ONLY)
        ge_error("ge target '${GE_TARGET_NAME}': cannot simultaneously have SOURCES and be HEADER_ONLY")
    endif()
    if(GE_TARGET_PROGRAM AND GE_TARGET_HEADER_ONLY)
        ge_error("ge target '${GE_TARGET_NAME}': a PROGRAM cannot be HEADER_ONLY")
    endif()

    if(GE_TARGET_HEADER_ONLY)
        add_library(${GE_TARGET_NAME} INTERFACE)
        target_include_directories(${GE_TARGET_NAME} INTERFACE include)

        if(GE_TARGET_DEPENDS)
            foreach(dep ${GE_TARGET_DEPENDS})
                target_link_libraries(${GE_TARGET_NAME} INTERFACE ${dep})
            endforeach()
        endif()
    elseif(GE_TARGET_SOURCES)
        if(GE_TARGET_PROGRAM)
            add_executable(${GE_TARGET_NAME} ${GE_TARGET_SOURCES})
        else()
            add_library(${GE_TARGET_NAME} ${GE_TARGET_SOURCES})
            target_include_directories(${GE_TARGET_NAME} PUBLIC include)
        endif()

        if(GE_TARGET_DEPENDS)
            foreach(dep ${GE_TARGET_DEPENDS})
                target_link_libraries(${GE_TARGET_NAME} PUBLIC ${dep})
            endforeach()
        endif()
    else()
        ge_error("'${GE_TARGET_NAME}' library must either have SOURCES or be HEADER_ONLY")
    endif()

    if(GE_TARGET_FLATBUFFERS)
        foreach(path ${GE_TARGET_FLATBUFFERS})
            set(source_fb_path "${CMAKE_CURRENT_SOURCE_DIR}/${path}")

            get_filename_component(name_base ${path} NAME_WLE)
            set(target_name "flatc_${name_base}")
            set(header_name "${name_base}_generated.h")
            set(header_path "${CMAKE_CURRENT_BINARY_DIR}/${header_name}")
            set(header_symlink_path "${CMAKE_CURRENT_SOURCE_DIR}/${header_name}")

            if(TARGET ${target_name})
                get_target_property(dir ${target_name} SOURCE_DIR)
                ge_error("ge target '${target_name}' already exists at '${dir}', rename '${path}'")
            endif()

            add_custom_command(
                COMMENT "generate header from ${source_fb_path}"
                OUTPUT ${header_path} ${header_symlink_path}
                DEPENDS ${source_fb_path}
                COMMAND
                    $<TARGET_FILE:flatc>
                        --cpp
                        -o "${CMAKE_CURRENT_BINARY_DIR}/"
                        ${source_fb_path}
                COMMAND
                    ${CMAKE_COMMAND} -E create_symlink
                        "${CMAKE_CURRENT_BINARY_DIR}/${header_name}"
                        "${CMAKE_CURRENT_SOURCE_DIR}/${header_name}"
            )
            add_custom_target(
                ${target_name}
                DEPENDS ${header_path} ${header_symlink_path}
            )
            add_dependencies(${GE_TARGET_NAME} ${target_name})
        endforeach()
    endif()
endmacro()

macro(ge_resource_library)
    set(options "")
    set(one_value_args NAME)
    set(multi_value_args "")
    cmake_parse_arguments(
        GE_RESOURCE_LIBRARY
            "${options}"
            "${one_value_args}"
            "${multi_value_args}"
            ${ARGN})

    file(RELATIVE_PATH relative_path ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

    set(input_args "")
    set(input_paths "")
    while(ARGN)
        list(POP_FRONT ARGN type)
        list(POP_FRONT ARGN path)
        list(POP_FRONT ARGN name)

        if(type STREQUAL "PNG")
            list(APPEND input_args --png "${path}" "${name}")
        elseif(type STREQUAL "TTF")
            list(APPEND input_args --ttf "${path}" "${name}")
        else()
            ge_error("unknown resource type: ${type}")
        endif()
        list(APPEND input_paths "${path}")
    endwhile()

    add_custom_command(
        COMMENT "generate resource library ${GE_RESOURCE_LIBRARY_NAME} at ${relative_path}"
        OUTPUT
            "${CMAKE_CURRENT_BINARY_DIR}/resources.hpp"
            "${CMAKE_CURRENT_BINARY_DIR}/data.ge"
        COMMAND
            $<TARGET_FILE:pack-resources>
                ${input_args}
                --output-header "${CMAKE_CURRENT_BINARY_DIR}/resources.hpp"
                --output-data "${CMAKE_CURRENT_BINARY_DIR}/data.ge"
        DEPENDS ${input_paths}
    )
    add_custom_target(
        ${GE_RESOURCE_LIBRARY_NAME}
        DEPENDS
            "${CMAKE_CURRENT_BINARY_DIR}/resources.hpp"
            "${CMAKE_CURRENT_BINARY_DIR}/data.ge"
    )
endmacro()
