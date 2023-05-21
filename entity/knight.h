#ifndef KNIGHT_H
#define KNIGHT_H

#include "entity.h"

class knight : protected entity
{
    knight();

    virtual bool move(int newX, int newY) override;
    virtual bool attack(int attX, int attY) override;
};

#endif