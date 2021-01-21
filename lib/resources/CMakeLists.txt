ge_target(
    DEPENDS
        error
        util

    FLATBUFFERS
        schema.fbs

    SOURCES
        resource_writer.cpp
        resources.cpp
)
