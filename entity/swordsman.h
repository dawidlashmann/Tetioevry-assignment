#ifndef SWORDSMAN_H
#define SWORDSMAN_H

#include "entity.h"

class swordsman : public entity
{
public:
    swordsman(int x, int y)
    {
        this->x = x;
        this->y = y;
        health = 60;
        speed = 2;
        cost = 250;
        attackRange = 1;
        buildTime = 3;
        unitType = 'S';
        attacked = false;
    };
};

#endif