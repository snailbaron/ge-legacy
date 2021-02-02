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
        set(generated_headers_dir "${CMAKE_CURRENT_BINARY_DIR}/generated_flatbuffers")
        if(GE_TARGET_HEADER_ONLY)
            target_include_directories(${GE_TARGET_NAME} INTERFACE ${generated_headers_dir})
            target_link_libraries(${GE_TARGET_NAME} INTERFACE flatbuffers)
        else()
            target_include_directories(${GE_TARGET_NAME} PUBLIC ${generated_headers_dir})
            target_link_libraries(${GE_TARGET_NAME} PUBLIC flatbuffers)
        endif()

        foreach(path ${GE_TARGET_FLATBUFFERS})
            set(source_fb_path "${CMAKE_CURRENT_SOURCE_DIR}/${path}")

            get_filename_component(name_base ${path} NAME_WLE)
            set(target_name "flatc_${name_base}")
            set(header_name "${name_base}_generated.h")
            set(header_path "${generated_headers_dir}/${header_name}")

            if(TARGET ${target_name})
                get_target_property(dir ${target_name} SOURCE_DIR)
                ge_error("ge target '${target_name}' already exists at '${dir}', rename '${path}'")
            endif()

            file(RELATIVE_PATH relative_source_fb_path ${CMAKE_SOURCE_DIR} ${source_fb_path})
            file(RELATIVE_PATH relative_header_path ${CMAKE_SOURCE_DIR} ${header_path})

            add_custom_command(
                COMMENT "generate header from ${relative_source_fb_path}: ${relative_header_path}"
                OUTPUT ${header_path}
                COMMAND
                    $<TARGET_FILE:flatc>
                        --cpp
                        -o "${generated_headers_dir}/"
                        ${source_fb_path}
                DEPENDS ${source_fb_path} flatc
            )
            add_custom_target(
                ${target_name}
                DEPENDS ${header_path}
            )

            add_dependencies(${GE_TARGET_NAME} ${target_name})
        endforeach()
    endif()

    if(GE_TARGET_RESOURCES)
        set(resource_list_path "${CMAKE_CURRENT_BINARY_DIR}/resource.list")
        if(NOT EXISTS ${resource_list_path})
            file(WRITE ${resource_list_path} "")
        endif()

        set(new_resource_list_path "${CMAKE_CURRENT_BINARY_DIR}/resource.list.new")
        file(WRITE ${new_resource_list_path} "")

        set(known_resource_types SPRITE FONT)
        set(resource_paths "")
        list(POP_FRONT GE_TARGET_RESOURCES resource_type)
        while(resource_type)
            if(NOT resource_type IN_LIST known_resource_types)
                ge_error("ge target '${GE_TARGET_NAME}': unknown resource type '${resource_type}'")
            endif()

            list(POP_FRONT GE_TARGET_RESOURCES resource_path)
            if(IS_ABSOLUTE ${resource_path})
                set(resource_path "${PROJECT_SOURCE_DIR}${resource_path}")
            else()
                set(resource_path "${CMAKE_CURRENT_SOURCE_DIR}/${resource_path}")
            endif()

            if(NOT EXISTS ${resource_path})
                ge_error("ge target '${GE_TARGET_NAME}': resource file does not exist: '${resource_path}'")
            endif()
            list(APPEND resource_paths ${resource_path})

            string(TOLOWER ${resource_type} resource_type_lc)
            set(resource_description "[${resource_type_lc}] path=${resource_path};")

            while(TRUE)
                list(POP_FRONT GE_TARGET_RESOURCES key_value)
                string(FIND "${key_value}" "=" equal_sign_position)
                if(equal_sign_position EQUAL -1)
                    set(resource_type ${key_value})
                    break()
                endif()
                math(EXPR value_position "${equal_sign_position} + 1")
                string(SUBSTRING ${key_value} 0 ${equal_sign_position} key)
                string(SUBSTRING ${key_value} ${value_position} -1 value)
                string(APPEND resource_description " ${key}=${value};")
            endwhile()
            ge_log("resource description: ${resource_description}")
            file(APPEND ${new_resource_list_path} "${resource_description}\n")
        endwhile()

        file(MD5 ${resource_list_path} resource_list_hash)
        file(MD5 ${new_resource_list_path} new_resource_list_hash)
        if(NOT resource_list_hash STREQUAL new_resource_list_hash)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${new_resource_list_path} ${resource_list_path})
        endif()

        set(resource_data_path "${CMAKE_CURRENT_BINARY_DIR}/resources.data")
        set(pack_resources_target_name "${GE_TARGET_NAME}-pack-resources")
        set(generated_header_dir "${CMAKE_CURRENT_BINARY_DIR}/generated_resource_header")
        file(MAKE_DIRECTORY ${generated_header_dir})
        set(generated_header_path "${generated_header_dir}/resource_ids.hpp")
        add_custom_command(
            COMMENT "pack resources for target '${GE_TARGET_NAME}'"
            OUTPUT ${resource_data_path}
            COMMAND $<TARGET_FILE:re>
                pack "${resource_list_path}" "${resource_data_path}" "${generated_header_path}"
            DEPENDS re ${resource_list_path} ${resource_paths}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )
        add_custom_target(
            ${pack_resources_target_name}
            DEPENDS ${resource_data_path} ${generated_header_path}
        )

        add_dependencies(${GE_TARGET_NAME} ${pack_resources_target_name} re)
        target_include_directories(${GE_TARGET_NAME} PUBLIC ${generated_header_dir})
    endif()
endmacro()
