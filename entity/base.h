#ifndef BASE_H
#define BASE_H

#include "entity.h"

class base_ : public entity
{
public:
    base_(int x, int y)
    {
        this->x = x;
        this->y = y;
        health = 200;
        speed = 0;
        unitType = 'B';
        buildTimeRemaining = -1;
        buildingEntity = ' ';
    };

    void build(char unitType, int time)
    {
        buildingEntity = unitType;
        buildTimeRemaining = time;
    }

    char get_building_type()
    {
        return buildingEntity;
    }

    int get_time_remaining()
    {
        return buildTimeRemaining;
    }

    void one_turn()
    {
        buildTimeRemaining--;
    }

private:
    int buildTimeRemaining;
    char buildingEntity;
};

#endif