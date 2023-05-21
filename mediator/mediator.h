#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <string>
#include "archer.h"
#include "base.h"
#include "catapult.h"
#include "knight.h"
#include "pikeman.h"
#include "ram.h"
#include "swordsman.h"
#include "worker.h"
#include "entity.h"

class mediator
{
public:
    mediator(const char *map, const char *status, const char *orders);

private:
    // update map based on given orders
    void update_map();
    // read the status file
    void update_status();
    // checks if the given orders are valid, and if so, executes them 
    void exec_orders();
    // create entity if needed
    entity *create_entity(char unitType, int x, int y);
    // checks how much damage a unit is dealing to diffrent unit
    int fight(char attacker, char defender);
    // checks if a unit can move to the given square (is it occupied)
    bool can_move(int x, int y);

private:
    const char *file_names[3];
    std::fstream map;
    std::fstream status;
    std::fstream orders;
    std::unordered_map<int, entity *> playerOneUnits;
    std::unordered_map<int, entity *> playerTwoUnits;
    // squares where a unit can't stay
    std::vector<std::pair<int, int>> obstacles;
    std::pair<int, int> mapSize{0, 0};
    // 1 - first player / 0 - second player
    bool turn;
};

#endif