# Tetioevry-assignment
### About
This project is a text based game, utilising `.txt` files as the board.

There are two programs (executables), which can be found in the `build/` directory: `game` acts as a mediator, while `player` is the player.

### Logic
Mediator, controls the game while progressing through it.

Player, based on the provided .txt files `map.txt` and `status.txt`, generates and outputs orders to the `order.txt` file.
The mediator then opens this file, and executes the orders, checking for their correctness, updates the `status.txt` file, and runs the player program again providing the updated files (as the opponent).

The player generates the orders as below:

Move:
* distance from the `enemy base` - during each move, the program wants to bring a unit closer to the enemy base.
* a move won't be chosen if there are more than two enemy units attacking the square, unless the number of enemies is lower than the current number.

Attack:
* attack is chosen if there are any `enemy units` in bound - if there are more than one enemy in range, the enemy with the lowest hp will be chosen to attack.
* if the `enemy base` is in range, it will always be chosen to be attacked

Build:
* if the player's base is not already during building a `unit`, it will start building the one with the fewest occurences, unless there isn't enough `gold` to afford it

### Using the code
* a basic map has been put inside `map.txt` with the other two left empty.
* the files have been put in a seperate directory `txt_files/`, to keep the repo clean and organized.
* if any files have been moved (the .txt files or the executables) the code has to be changed accordingly. 
* the file names are inside `main_mediator.cpp` where they can be modified. Be aware that the `game` program runs `player` through Linux command system with `args` as the needed file names. Thus after any changes, it is crucial to make sure, that the correct name and path to the files (relative to `player`) is given inside the `game_begin()` method
* to use a diffrent program as the player/s, please refer to the `game_begin()` method inside `mediator.cpp`
* `CMakeLists.txt` file is provided for using cmake, as well as `Makefile` file inside `build/` direcotry for automatic building
* if any changes have been made to the files placement, the path needs to be updated there
* additionaly, `CMakeLists.txt` file is linking to the `entity/` directory, thus the path needs to be changed accordingly to the changes made in the repo

In case of any issues, please contact me any way possible.
