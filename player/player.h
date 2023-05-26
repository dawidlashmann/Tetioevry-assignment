#ifndef PLAYER_H
#define PLAYER_H

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>
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

private:
    entity *create_entity(char unitType, int x, int y);

private:
    std::ifstream map, status;
    std::ofstream orders;
    std::unordered_map<int, entity *> ownUnits;
    std::unordered_map<int, entity *> enemyUnits;
    std::vector<std::pair<int, int>> obstacles;
    std::pair<int, int> mapSize{0, 0};
    base_ *ownBase;
    base_ *enemyBase;
    int gold = -1;
};

#endif