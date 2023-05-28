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
    void runWithTimeout(float runtime);

    void get_orders();
private:
    // runs the below methods and prints the orders to the "orders.txt" file
    // generates the best move
    // the best move is determined by the lowest distance remaining from the unit and enemy base
    // the best move wont be chosen if at given coordinates more than 2 enemy units can attack the unit
    std::pair<int, int> get_move(entity *unit);
    // returns ID of attacked enemy unit
    // if there are more than one enemy units in bound, the ID of a unit with the lowest hp is returned
    int get_attack(entity *unit);
    // returns unit type to build
    char build();
    entity *create_entity(char unitType, int x, int y);
    int fight(char attacker, char defender);

private:
    std::ifstream map, status;
    std::ofstream orders;
    std::unordered_map<int, entity *> ownUnits;
    std::unordered_map<int, entity *> enemyUnits;
    std::vector<std::pair<int, int>> obstacles;
    std::vector<std::pair<int, int>> mines;
    std::pair<int, int> mapSize{0, 0};
    std::pair<int, base_ *> ownBase;
    std::pair<int, base_ *> enemyBase;
    int gold = -1;
    bool finished = false;
};

#endif