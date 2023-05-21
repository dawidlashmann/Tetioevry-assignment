#include "player.h"

player::player(const char *map, const char *status, const char *orders)
{
    this->map.open(map);
    this->status.open(status);
    this->orders.open(orders, std::ios::out);

    if (!this->map.is_open() || !this->status.is_open() || !this->orders.is_open())
    {
        std::cout << "Wrong input\nExiting\n";
    }
}