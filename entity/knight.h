#ifndef KNIGHT_H
#define KNIGHT_H

#include "entity.h"

class knight : public entity
{
public:
    knight(int x, int y)
    {
        this->x = x;
        this->y = y;
        health = 70;
        speed = 5;
        cost = 400;
        attackRange = 1;
        buildTime = 5;
        unitType = 'K';
        attacked = false;
    };
};

#endif