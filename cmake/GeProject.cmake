include(GeMessages)

macro(ge_library)
    set(options HEADER_ONLY)
    set(one_value_args NAME)
    set(multi_value_args SOURCES)
    cmake_parse_arguments(
        GE_LIBRARY
            "${options}"
            "${one_value_args}"
            "${multi_value_args}"
            ${ARGN})

    set(call_text "ge_library(${ARGV})")
    string(REPLACE ";" " " call_text "${call_text}")

    require(
        "Unknown arguments ${GE_LIBRARY_UNPARSED_ARGUMENTS}\n ${call_text}"
        NOT GE_LIBRARY_UNPARSED_ARGUMENTS)

    require(
        "No values: ${GE_LIBRARY_KEYWORDS_MISSING_VALUES}\n ${call_text}"
        NOT GE_LIBRARY_KEYWORDS_MISSING_VALUES)

    require(
        "Library must have a NAME:\n ${call_text}"
        GE_LIBRARY_NAME)

    require(
        "Library must either have SOURCES or be HEADER_ONLY:\n ${call_text}"
        GE_LIBRARY_HEADER_ONLY OR GE_LIBRARY_SOURCES)

    if(GE_LIBRARY_HEADER_ONLY)
        add_library(${GE_LIBRARY_NAME} INTERFACE)
        target_include_directories(
            ${GE_LIBRARY_NAME} INTERFACE include)
    else()
        add_library(${GE_LIBRARY_NAME} ${GE_LIBRARY_SOURCES})
        target_include_directories(${GE_LIBRARY_NAME} PUBLIC include)
    endif()
endmacro()
