#ifndef RAM_H
#define RAM_H

#include "entity.h"

class ram : public entity
{
public:
    ram(int x, int y)
    {
        this->x = x;
        this->y = y;
        health = 90;
        speed = 2;
        cost = 500;
        attackRange = 1;
        buildTime = 4;
        unitType = 'R';
        attacked = false;
    };
};

#endif