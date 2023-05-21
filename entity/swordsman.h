#ifndef SWORDSMAN_H
#define SWORDSMAN_H

#include "entity.h"

class swordsman : protected entity
{
public:
    swordsman()
    {
        x = 0;
        y = 0;
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