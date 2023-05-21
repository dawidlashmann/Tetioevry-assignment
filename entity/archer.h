#ifndef ARCHER_H
#define ARCHER_H

#include "entity.h"

class archer : public entity
{
public:
    archer(int x, int y)
    {
        this->x = x;
        this->y = y;
        health = 40;
        speed = 2;
        cost = 250;
        attackRange = 5;
        buildTime = 3;
        unitType = 'A';
        attacked = false;
    };
};

#endif