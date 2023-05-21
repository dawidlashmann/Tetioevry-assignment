#include "entity.h"

bool entity::move(int newX, int newY)
{
    int range = abs(newX - x) + abs(newY - y);
    if (range > speed)
    {
        return false;
    }
    speed -= range;
    return true;
}

bool entity::attack(int attX, int attY)
{
    // one attack per turn
    if (attacked || speed < 1)
        return false;

    int range = abs(attX - x) + abs(attY - y);
    if (range > attackRange)
    {
        return false;
    }
    speed -= 1;
    attacked = true;
    return true;
}