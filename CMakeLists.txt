cmake_minimum_required(VERSION 3.0)
project(game)

# Add the sources of the program
set(SOURCES 
    main.cpp
)

# Add the executable for the program
add_executable(game ${SOURCES})