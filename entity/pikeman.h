#ifndef PIKEMAN_H
#define PIKEMAN_H

#include "entity.h"

class pikeman : public entity
{
public:
    pikeman(int x, int y)
    {
        this->x = x;
        this->y = y;
        health = 50;
        speed = 2;
        cost = 200;
        attackRange = 2;
        buildTime = 3;
        unitType = 'P';
        attacked = false;
    };
};

#endif