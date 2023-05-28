#ifndef PLAYER_H
#define PLAYER_H

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
#include <thread>
#include <chrono>
#include "../entity/archer.h"
#include "../entity/base.h"
#include "../entity/catapult.h"
#include "../entity/knight.h"
#include "../entity/pikeman.h"
#include "../entity/ram.h"
#include "../entity/swordsman.h"
#include "../entity/worker.h"
#include "../entity/entity.h"

class player
{
public:
    player(const char *map_name, const char *status_name, const char *orders_name);
    ~player();

    // uses multithreading to check if the program has exceeded the given time limit
    // this function creates a thread to run the get_orders() method, while the other checks how much time has passed
    // and terminates if it was exceeded
    void runWithTimeout(float runtime);

private:
    // runs the below methods and prints the orders to the "orders.txt" file
    void get_orders();

    // generates the best move
    // the best move is determined by the lowest distance remaining from the unit and enemy base/mine
    // a move wont be chosen if at given coordinates more than 2 enemy units can attack the unit
    std::pair<int, int> get_move(entity *unit);

    // returns ID of attacked enemy unit
    // if there are more than one enemy units in bound, the ID of a unit with the lowest hp is returned
    // if no enemy units in bound, return -1
    int get_attack(entity *unit);

    // returns unit type, a char, to build by the base
    // if a base is already building, returns '0'
    // picks a unit of the fewest number (if can afford)
    char build();

    // returns a pointer to a created entity
    entity *create_entity(char unitType, int x, int y);

    // returns a value of damage dealt
    int fight(char attacker, char defender);

private:
    std::ifstream map, status;
    std::ofstream orders;
    std::unordered_map<int, entity *> ownUnits;
    std::unordered_map<int, entity *> enemyUnits;
    std::vector<std::pair<int, int>> obstacles;
    std::vector<std::pair<int, int>> mines;
    std::pair<int, int> mapSize{0, 0};
    // holds the ID of base and it's pointer
    std::pair<int, base_ *> ownBase;
    std::pair<int, base_ *> enemyBase;

    int gold = -1;

    // is true, when a thread executing get_orders() finishes the job
    bool finished = false;
};

#endif