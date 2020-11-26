ge_library(
    SOURCES
        packer.cpp

    FLATBUFFERS
        schema.fbs

    DEPENDS
        error
        util
)
