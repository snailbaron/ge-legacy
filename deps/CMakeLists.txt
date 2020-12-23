set(ENABLE_NEWS OFF CACHE BOOL "")
set(ENABLE_SCRIPTING OFF CACHE BOOL "")
set(ENABLE_UI OFF CACHE BOOL "")
set(ENABLE_UPDATER OFF CACHE BOOL "")
set(WITH_WEBP_SUPPORT OFF CACHE BOOL "")
add_subdirectory(aseprite)

add_subdirectory(Catch2)
add_subdirectory(SFML)
add_subdirectory(arg)
add_subdirectory(flatbuffers)
add_subdirectory(json)

include(FetchContent)
FetchContent_Declare(
    LIBPNG
    URL "https://download.sourceforge.net/libpng/libpng-1.6.37.tar.xz"
)
