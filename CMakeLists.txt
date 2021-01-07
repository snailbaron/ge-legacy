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

add_subdirectory(deps)

if(NOT MSVC)
    add_compile_options(-Werror)
endif()

set(GE_CPPCHECK "" CACHE STRING "Value of CMAKE_CXX_CPPCHECK to set before building ge")
if(GE_CPPCHECK)
    set(CMAKE_CXX_CPPCHECK ${GE_CPPCHECK})
endif()

set(GE_CLANG_TIDY "" CACHE STRING "Value of CMAKE_CXX_CLANG_TIDY to set before building ge")
if(GE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY ${GE_CLANG_TIDY})
endif()

add_subdirectory(examples)
add_subdirectory(lib)
add_subdirectory(tools)
