#include "player.h"

player::player(const char *map_name, const char *status_name, const char *orders_name)
{
    this->map.open(map_name);
    this->status.open(status_name);
    this->orders.open(orders_name, std::ios::out);

    if (!this->map.is_open() || !this->status.is_open() || !this->orders.is_open())
    {
        std::cout << "Wrong input\nExiting\n";
    }

    // get map size and the obstacles' position
    std::string line;
    while (getline(this->map, line))
    {
        for (int col = 0; col < line.length(); col++)
        {
            if (line[col] == '9')
            {
                obstacles.emplace_back(col, mapSize.second);
            }
        }
        if (mapSize.first == 0)
        {
            mapSize.first = line.length();
        }
        mapSize.second++;
    }

    // ownUnits.reserve(999);
    // enemyUnits.reserve(999);

    // set units of both player, and the enemy based on the status file
    while (std::getline(this->status, line))
    {
        if (gold == -1)
        {
            gold = std::stoi(line);
            continue;
        }

        std::istringstream iss(line);
        std::vector<std::string> unitStatus;

        std::string status_;
        while (iss >> status_)
        {
            unitStatus.emplace_back(status_);
        }

        for (auto status : unitStatus)
        {
            auto &units = (unitStatus[0].c_str()[0] == 'P') ? ownUnits : enemyUnits;
            entity *newEntity = create_entity(unitStatus[1].c_str()[0], std::stoi(unitStatus[3]), std::stoi(unitStatus[4]));
            char newHp = std::stoi(unitStatus[5]);
            newEntity->set_health(std::stoi(unitStatus[5]));
            if (unitStatus[1].c_str()[0] == 'B')
            {
                if (unitStatus[0].c_str()[0] == 'P')
                {
                    ownBase = static_cast<base_ *>(newEntity);
                    if (unitStatus[6].c_str()[0] != '0')
                    {
                        ownBase->build(unitStatus[6].c_str()[0], 1);
                    }
                }
                else
                {
                    enemyBase = static_cast<base_ *>(newEntity);
                }
            }
            else
            {
                units[unitStatus[2].c_str()[0]] = newEntity;
            }
        }
    }
}

player::~player()
{
    map.close();
    status.close();
    orders.close();
}

entity *player::create_entity(char unitType, int x, int y)
{
    entity *newUnit = nullptr;
    switch (unitType)
    {
    case 'A':
        newUnit = new archer(x, y);
        break;
    case 'C':
        newUnit = new catapult(x, y);
        break;
    case 'K':
        newUnit = new knight(x, y);
        break;
    case 'P':
        newUnit = new pikeman(x, y);
        break;
    case 'R':
        newUnit = new ram(x, y);
        break;
    case 'S':
        newUnit = new swordsman(x, y);
        break;
    case 'W':
        newUnit = new worker(x, y);
        break;
    case 'B':
        newUnit = new base_(x, y);
        break;
    }

    return newUnit;
}