#ifndef KNIGHT_H
#define KNIGHT_H

#include "entity.h"

class knight : protected entity
{
public:
    knight()
    {
        x = 0;
        y = 0;
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