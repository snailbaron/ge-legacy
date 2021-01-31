ge_target(
    PROGRAM

    DEPENDS
        ge
        util

    RESOURCES
        SPRITE data/rock.png
        SPRITE data/ship.png frames=2
        FONT data/OpenSans-Regular.ttf id=OpenSans

    SOURCES
        main.cpp
)
