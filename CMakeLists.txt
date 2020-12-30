cmake_minimum_required(VERSION 3.0)

project(ge)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(Ge)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only). Useful for ninja build." FALSE)
if(${FORCE_COLORED_OUTPUT})
    add_compile_options(
      $<$<CXX_COMPILER_ID:GNU>:-fdiagnostics-color=always>
      $<$<CXX_COMPILER_ID:Clang>:-fcolor-diagnostics>
    )
endif()

add_subdirectory(deps)

if(NOT MSVC)
    add_compile_options(-Werror)
endif()
add_subdirectory(examples)
add_subdirectory(lib)
add_subdirectory(tools)
