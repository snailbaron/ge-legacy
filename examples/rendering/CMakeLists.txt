ge_resource_library(
    NAME resources
    PNG data/grass.png Grass
)

ge_program(
    NAME rendering

    SOURCES
        main.cpp

    RESOURCES
        PNG data/grass.png Grass
        PNG data/tree.png Tree
        TTF data/OpenSans-Regular.ttf OpenSans
)
