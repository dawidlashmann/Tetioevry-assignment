#ifndef BASE_H
#define BASE_H

#include "entity.h"

class base : protected entity
{
public:
    base(int x, int y)
    {
        this->x = x;
        this->y = y;
        health = 200;
        speed = 0;
        unitType = 'B';
        buildTimeRemaining = -1;
    };

private:
    int buildTimeRemaining;
};

#endif