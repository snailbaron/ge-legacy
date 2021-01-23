ge_target(
    PROGRAM

    DEPENDS
        ge

    RESOURCES
        PNG data/rock.png Rock
        PNG data/ship.png Ship FRAMES 2
        TTF data/OpenSans-Regular.ttf OpenSans

    SOURCES
        main.cpp
)
