add_subdirectory(Catch2)
add_subdirectory(SFML)
add_subdirectory(arg)

set(FLATBUFFERS_BUILD_TESTS OFF)
add_subdirectory(flatbuffers)

set(JSON_BuildTests OFF)
add_subdirectory(json)

include(FetchContent)
FetchContent_Declare(
    LIBPNG
    URL "https://download.sourceforge.net/libpng/libpng-1.6.37.tar.xz"
)
