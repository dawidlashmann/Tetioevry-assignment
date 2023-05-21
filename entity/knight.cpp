#include "knight.h"

knight::knight()
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
}

bool knight::move(int newX, int newY)
{
    int range = abs(newX - x) + abs(newY - y);
    if (range > speed)
    {
        return false;
    }
    speed -= range;
    return true;
}

bool knight::attack(int attX, int attY)
{
    // one attack per turn
    if(attacked)
        return false;

    int range = abs(attX - x) + abs(attY - y);
    if (range > attackRange)
    {
        return false;
    }
    
    attacked = true;
    return true;
}