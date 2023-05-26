#include <iostream>
#include "mediator.h"

int main()
{
    mediator med("map.txt", "status.txt", "orders.txt");
    med.game_begin();

    return 0;
}