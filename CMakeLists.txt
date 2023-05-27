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
    player/player.cpp
    player/main_player.cpp
    entity/entity.cpp
)

# Find the required threads library
find_package(Threads REQUIRED)

# Add the executable for the mediator
add_executable(game ${SOURCES1})

# Add the executable for the player
add_executable(player ${SOURCES2})

# Link the executable with the Threads library
target_link_libraries(game PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(player PRIVATE ${CMAKE_THREAD_LIBS_INIT})

# Include the header files in entity directory
target_include_directories(game PRIVATE entity)
target_include_directories(player PRIVATE entity)
