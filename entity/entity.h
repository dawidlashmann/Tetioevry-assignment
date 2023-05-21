#ifndef ENTITY_H
#define ENTITY_H

#include "cmath"

class entity
{
public:
    entity(){};

    // this function checks if the entity can move to the given coordinates
    // it does not check if the coordinates are occupied or even exist
    bool move(int newX, int newY);
    // this function checks if the entity has the range to attack given coordinates
    // it does not check if the coordinates are occupied by the enemy entity
    bool attack(int attX, int attY);

protected:
    // current position
    int x, y;

    int health;
    int speed;
    int cost;
    int attackRange;
    int buildTime;
    char unitType;
    // only one attack per turn is allowed
    bool attacked;
};

#endif