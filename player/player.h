#ifndef PLAYER_H
#define PLAYER_H

#include <fstream>
#include <iostream>

class player
{
public:
    player(const char *map, const char *status, const char *orders);

private:
    std::ifstream map, status;
    std::ofstream orders;
};

#endif