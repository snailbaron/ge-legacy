ge_target(
    PROGRAM

    DEPENDS
        ge
        util

    RESOURCES
        SPRITE data/rock.png
        SPRITE data/ship.png FRAMES 2
        FONT data/OpenSans-Regular.ttf ID OpenSans

    SOURCES
        main.cpp
)
