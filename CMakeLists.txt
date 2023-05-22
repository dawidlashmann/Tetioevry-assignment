cmake_minimum_required(VERSION 3.0)
project(game)

# Add the sources of the mediator program
set(SOURCES1
    mediator/mediator.cpp
    mediator/main_mediator.cpp
    entity/entity.cpp
)

# Add the sources of the player program
set(SOURCES2
    mediator/player.cpp
    mediator/main_player.cpp
    entity/entity.cpp
)

# Add the executable for the mediator
add_executable(game ${SOURCES1})

# Add the executable for the player
add_executable(player ${SOURCES2})

#include the header files in entity directory
target_include_directories(game PRIVATE entity)