ge_target(
    DEPENDS
        error
        util

    FLATBUFFERS
        schema.fbs

    SOURCES
        storage.cpp
        writer.cpp
)
