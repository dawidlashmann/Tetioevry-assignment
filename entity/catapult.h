#ifndef CATAPULT_H
#define CATAPULT_H

#include "entity.h"

class catapult : public entity
{
public:
    catapult(int x, int y)
    {
        this->x = x;
        this->y = y;
        health = 50;
        speed = 2;
        cost = 800;
        attackRange = 7;
        buildTime = 6;
        unitType = 'C';
        attacked = false;
    };
};

#endif