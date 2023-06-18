#include "entity.h"

bool entity::move(int newX, int newY)
{
    int range = abs(newX - x) + abs(newY - y);
    if (range > currentSpeed)
    {
        return false;
    }
    currentSpeed -= range;
    this->x = newX;
    this->y = newY;
    return true;
}

bool entity::attack(int attX, int attY)
{
    // one attack per turn
    if (attacked || currentSpeed < 1)
        return false;

    int range = abs(attX - x) + abs(attY - y);
    if (range > attackRange)
    {
        return false;
    }
    currentSpeed -= 1;
    attacked = true;
    return true;
}

bool entity::can_attack(int attX, int attY)
{
    if (attacked || currentSpeed < 1)
        return false;

    int range = abs(attX - x) + abs(attY - y);
    if (range > attackRange)
    {
        return false;
    }
    return true;
}

void entity::damage(int dmg)
{
    this->health -= dmg;
}

std::pair<int, int> entity::get_position()
{
    return std::pair<int, int>{this->x, this->y};
}

void entity::next_turn()
{
    currentSpeed = speed;
    attacked = false;
}