#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <cstdlib>
#include <iostream>
#include <fstream>

class mediator
{
public:
    mediator(const char *map, const char *status, const char *orders);

private:
    // update map based on given orders
    void update_map();

private:
    std::fstream map;
    std::fstream status;
    std::fstream orders;
};

#endif