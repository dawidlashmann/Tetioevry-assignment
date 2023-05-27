#include "player.h"

player::player(const char *map_name, const char *status_name, const char *orders_name)
{
    this->map.open(map_name);
    this->status.open(status_name);
    this->orders.open(orders_name, std::ios::out);

    if (!this->map.is_open() || !this->status.is_open() || !this->orders.is_open())
    {
        throw "Wrong input\nExiting\n";
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
            else if (line[col] == '6')
            {
                mines.emplace_back(col, mapSize.second);
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
            int newHp = std::stoi(unitStatus[5]);
            newEntity->set_health(newHp);
            if (unitStatus[1].c_str()[0] == 'B')
            {
                if (unitStatus[0].c_str()[0] == 'P')
                {
                    ownBase.first = std::stoi(unitStatus[2]);
                    ownBase.second = static_cast<base_ *>(newEntity);
                    ownBase.second->build(unitStatus[6].c_str()[0], 1);
                }
                else
                {
                    enemyBase.first = std::stoi(unitStatus[2]);
                    enemyBase.second = static_cast<base_ *>(newEntity);
                }
            }
            else
            {
                units[std::stoi(unitStatus[2])] = newEntity;
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

void player::runWithTimeout(float runtime)
{
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds timeout(static_cast<long>(runtime * 1000));

    std::thread t(&player::get_orders, this);

    auto now = std::chrono::high_resolution_clock::now();
    while (now - start_time <= timeout)
    {
        if (!t.joinable())
        {
            t.join();
            return;
        }
        now = std::chrono::high_resolution_clock::now();
    }

    throw std::runtime_error("Time limit exceeded");
}

void player::get_orders()
{
    {
        char buildUnit = build();
        if (buildUnit != '0')
            orders << ownBase.first << " B " << buildUnit << '\n';
    }

    for (auto unit : ownUnits)
    {
        auto attackID = get_attack(unit.second);

        auto unitMove = get_move(unit.second);

        if (attackID == -1)
            attackID = get_attack(unit.second);

        if (unitMove.first != -1 && unitMove.second != -1)
            orders << unit.first << " M " << unitMove.first << " " << unitMove.second << '\n';

        if (attackID != -1)
            orders << unit.first << " A " << attackID << '\n';
    }
}

std::pair<int, int> player::get_move(entity *unit)
{

    const std::pair<int, int> &unitCoords = unit->get_position();
    int movesAvaiable = (!unit->attacked_()) ? unit->get_speed() - 1 : unit->get_speed();

    auto enemyBaseCoords = enemyBase.second->get_position();
    int statringDistance = abs(enemyBaseCoords.first - unitCoords.first) + abs(enemyBaseCoords.second - unitCoords.second);
    std::pair<int, std::pair<int, int>> bestMove{statringDistance, {unitCoords.first, unitCoords.second}};

    int attackers = 0;
    for (const auto &enemyUnit : enemyUnits)
    {
        if (enemyUnit.second->attack(unitCoords.first, unitCoords.second))
            attackers++;
    }

    std::pair<int, int> closestMine{-1, -1};
    if (unit->get_type() == 'W')
    {
        int shortestDistance = mapSize.first + mapSize.second;
        for (const auto &mine : mines)
        {
            int currentDistance = abs(mine.first - unitCoords.first) + abs(mine.second - unitCoords.second);
            if (currentDistance < shortestDistance)
            {
                shortestDistance = currentDistance;
                bestMove.first = shortestDistance;
                closestMine = mine;
            }
        }
    }

    std::queue<std::pair<int, int>> adjacentSquares;
    std::vector<std::pair<int, int>> visited;
    adjacentSquares.emplace(unitCoords.first, unitCoords.second);

    while (!adjacentSquares.empty())
    {
        int moveX = adjacentSquares.front().first;
        int moveY = adjacentSquares.front().second;
        adjacentSquares.pop();

        visited.emplace_back(moveX, moveY);

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                    continue;

                if ((abs(unitCoords.first - (moveX + i)) + abs(unitCoords.second - (moveY + j))) > movesAvaiable)
                    continue;

                std::pair<int, int> nextSquare{moveX + i, moveY + j};
                bool next = false;
                for (auto previous : visited)
                {
                    if (previous == nextSquare)
                    {
                        next = true;
                        break;
                    }
                }
                if (next)
                    continue;

                adjacentSquares.emplace(moveX + i, moveY + j);
            }
        }

        for (const auto &enemyUnit : enemyUnits)
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

        if (enemyBaseCoords.first == moveX && enemyBaseCoords.second == moveY)
            continue;

        if (moveX >= mapSize.first || moveY >= mapSize.second || moveX < 0 || moveY < 0)
            continue;

        int currentDistance;
        if (unit->get_type() == 'W' && closestMine.first != -1)
        {
            currentDistance = abs(closestMine.first - moveX) + abs(closestMine.second - moveY);
        }
        else
        {
            currentDistance = abs(enemyBaseCoords.first - moveX) + abs(enemyBaseCoords.second - moveY);
        }

        int currentAttackers = 0;
        for (const auto &enemyUnit : enemyUnits)
        {
            if (enemyUnit.second->attack(moveX, moveY))
                currentAttackers++;
        }
        if (currentAttackers <= 2 || (currentAttackers > 2 && attackers > currentAttackers))
        {
            if (bestMove.first > currentDistance)
            {
                bestMove.first = currentDistance;
                bestMove.second = std::pair<int, int>{moveX, moveY};
            }
        }
    }

    if (bestMove.second == unitCoords)
    {
        bestMove.second.first = -1;
        bestMove.second.second = -1;
    }

    return bestMove.second;
}

int player::get_attack(entity *unit)
{
    int ID = -1;
    int lowestHealth = 100;
    {
        auto enemyBaseCoords = enemyBase.second->get_position();
        if (unit->attack(enemyBaseCoords.first, enemyBaseCoords.second))
        {
            return enemyBase.first;
        }
    }
    for (const auto &enemyUnit : enemyUnits)
    {
        auto enemyUnitCoords = enemyUnit.second->get_position();
        if (unit->attack(enemyUnitCoords.first, enemyUnitCoords.second))
        {
            int unitHp = enemyUnit.second->get_health();
            if (lowestHealth > unitHp)
            {
                ID = enemyUnit.first;
                lowestHealth = unitHp;
            }
        }
    }

    return ID;
}

char player::build()
{
    if (ownBase.second->get_building_type() != '0')
        return '0';

    std::unordered_map<char, int>
        unitNumber({{'K', 0},
                    {'S', 0},
                    {'A', 0},
                    {'P', 0},
                    {'C', 0},
                    {'R', 0},
                    {'W', 0}});
    unitNumber.reserve(7);
    for (const auto &unit : ownUnits)
    {
        char unitType = unit.second->get_type();
        unitNumber[unitType]++;
    }

    std::vector<char> fewestToMost;
    fewestToMost.reserve(7);
    int lowestNum = 1000;
    for (const auto &num : unitNumber)
    {
        if (lowestNum >= num.second)
        {
            lowestNum = num.second;
            fewestToMost.emplace(fewestToMost.begin(), num.first);
        }
    }

    char result = '0';
    for (auto unit : fewestToMost)
    {
        entity *dummyEntity = create_entity(unit, 0, 0);
        if (gold >= dummyEntity->get_cost())
        {
            result = unit;
            break;
        }
        delete dummyEntity;
    }

    return result;
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