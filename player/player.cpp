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

void player::get_orders()
{
    for (auto unit : ownUnits)
    {
        auto unitMove = move(unit.second);
    }
}

std::pair<int, int> player::move(entity *unit)
{
    const std::pair<int, int> &unitCoords = unit->get_position();
    int moveX, moveY;
    int movesAvaiable = unit->get_speed() - 1;

    // "best" move case
    {
        const std::pair<int, int> &enemyBaseCoords = enemyBase->get_position();

        int distanceX = abs(unitCoords.first - enemyBaseCoords.first);
        int distanceY = abs(unitCoords.second - enemyBaseCoords.second);
        int distance = distanceX + distanceY;

        float distanceRatioX = static_cast<float>(distanceX) / static_cast<float>(distance);
        float distanceRatioY = static_cast<float>(distanceY) / static_cast<float>(distance);
        moveX = static_cast<int>(distanceRatioX * movesAvaiable);
        moveY = static_cast<int>(distanceRatioY * movesAvaiable);
    }

    std::vector<std::pair<int, std::pair<int, int>>> legalMoves;

    std::queue<std::pair<int, int>> adjacentSquares;
    adjacentSquares.emplace(unitCoords.first + moveX, unitCoords.second + moveY);

    bool isMoveLegal = false;
    while (!adjacentSquares.empty())
    {
        moveX = adjacentSquares.front().first;
        moveY = adjacentSquares.front().second;
        adjacentSquares.pop();

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                    continue;

                if (((moveX + i) + (moveY + j)) >= movesAvaiable)
                    continue;

                adjacentSquares.emplace(moveX + i, moveY + j);
            }
        }

        for (auto enemyUnit : enemyUnits)
        {
            auto enemyCoords = enemyUnit.second->get_position();
            if (enemyCoords.first == moveX && enemyCoords.second == moveY)
                continue;
        }

        for (const auto &obstacle : obstacles)
        {
            if (obstacle.first == moveX && obstacle.second == moveY)
                continue;
        }

        auto enemyBaseCoords = enemyBase->get_position();
        if (enemyBaseCoords.first == moveX && enemyBaseCoords.second == moveY)
            continue;

        if (moveX >= mapSize.first || moveY >= mapSize.second || moveX < 0 || moveY < 0)
            continue;

        legalMoves.emplace_back(((unitCoords.first - moveX) + (unitCoords.second - moveY)), (moveX, moveY));
    }

    for (auto enemyUnit : enemyUnits)
    {
        for (auto &move : legalMoves)
        {
            if (enemyUnit.second->attack(move.second.first, move.second.second))
            {
                move.first -= fight(enemyUnit.second->get_type(), unit->get_type());
            }
        }
    }

    return std::pair<int, int>{moveX, moveY};
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

int player::fight(char attacker, char defender)
{
    int damage;
    switch (attacker)
    {
    case 'A':
        damage = (defender == 'C' || defender == 'R') ? 10 : 15;
        break;
    case 'C':
        damage = (defender == 'B') ? 50 : 40;
        break;
    case 'K':
        damage = (defender == 'R') ? 50 : 35;
        break;
    case 'P':
        damage = (defender == 'B' || defender == 'R') ? 10 : 15;
        damage = (defender == 'K') ? 35 : damage;
        break;
    case 'R':
        damage = (defender == 'B') ? 50 : 10;
        break;
    case 'S':
        damage = (defender == 'P' || defender == 'C') ? 20 : 30;
        break;
    case 'W':
        damage = (defender == 'B') ? 1 : 5;
        break;
    }
    return damage;
}