#include <iostream>
#include "mediator.h"

int main()
{
    mediator med("../txt_files/map.txt", "../txt_files/status.txt", "../txt_files/orders.txt");
    med.game_begin();

    return 0;
}