#include "mediator.h"

mediator::mediator(const char *map, const char *status, const char *orders)
{
    turn = 1;
    // remeber files names for reausage later on
    file_names[0] = map;
    file_names[1] = status;
    file_names[2] = orders;

    // open .txt files
    this->map.open(map);
    this->status.open(status);

    if (!this->map.is_open() || !this->status.is_open())
    {
        std::cout << "Wrong input";
    }

    // get map size and the obstacles' coordinates
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
            mapSize.second = line.length();
        }
        mapSize.second++;
    }

    // set file flag to the beggining
    this->map.clear();
    this->map.seekg(0, std::ios::beg);

    // set the bases
    playerOneUnits.insert(std::make_pair(0, new base_(0, 0)));
    playerTwoUnits.insert(std::make_pair(0, new base_(mapSize.first, mapSize.second)));
}

mediator::~mediator()
{
    map.close();
    status.close();
    orders.close();
}

void mediator::update_status()
{
    auto &playerUnits = (turn) ? playerOneUnits : playerTwoUnits;
    auto &enemyUnits = (turn) ? playerTwoUnits : playerOneUnits;
    auto &playerGold = (turn) ? gold.first : gold.second;

    if (!status.is_open())
        return;

    status.close();
    status.open(file_names[1], std::ios::trunc);
    status << playerGold << '\n';
    for (auto unit : playerUnits)
    {
        std::pair<int, int> coords = unit.second->get_position();
        status << "P " << unit.second->get_type() << " " << unit.first << " " << coords.first << " " << coords.second << " " << unit.second->get_health();
        if (unit.second->get_type() == 'B')
        {
            base_ *base = static_cast<base_ *>(playerUnits[0]);
            status << " " << base->get_building_type();
        }
        status << '\n';
    }
    for (auto enemyUnit : enemyUnits)
    {
        std::pair<int, int> coords = enemyUnit.second->get_position();
        status << "E " << enemyUnit.second->get_type() << " " << enemyUnit.first << " " << coords.first << " " << coords.second << " " << enemyUnit.second->get_health();
        if (enemyUnit.second->get_type() == 'B')
        {
            base_ *base = static_cast<base_ *>(playerUnits[0]);
            status << " " << base->get_building_type();
        }
        status << '\n';
    }
}

void mediator::exec_orders()
{
    if (!orders.is_open())
        return;
    // get the order line text and divide it into seperate words
    std::string order;
    while (getline(orders, order))
    {
        if (order.length() >= 3)
        {
            std::istringstream iss(order);
            std::string word;

            // order but word by word
            std::vector<std::string> owbw;
            while (iss >> word)
            {
                owbw.push_back(word);
            }

            // use player list based on which player's turn is now
            auto &playerUnits = (turn) ? playerOneUnits : playerTwoUnits;
            auto &enemyUnits = (turn) ? playerTwoUnits : playerOneUnits;

            // get particular unit using the provided ID
            // first check if a unit with that ID even exists to prevent any memory leaks
            entity *unit;
            if (playerUnits.find(std::stoi(owbw[0])) != playerUnits.end())
            {
                unit = playerUnits[std::stoi(owbw[0])];
            }
            else
            {
                throw "Unit with provided ID does not exists";
            }

            // perform an action based on given order
            if (owbw[1] == "M")
            {
                // has to check if the space if occupied
                if (can_move(std::stoi(owbw[2]), std::stoi(owbw[3])))
                {
                    if (!unit->move(std::stoi(owbw[2]), std::stoi(owbw[3])))
                    {
                        throw "Given move is not valid";
                    };
                }
                else
                {
                    throw "Given move is not valid";
                }
            }
            else if (owbw[1] == "A")
            {
                // check if an enemy unit with provided ID exists in the enemy unit structure
                // doing that we do not have to check if we are attacking our own unit or an empty square
                entity *enemyUnit;
                if (enemyUnits.find(std::stoi(owbw[2])) != enemyUnits.end())
                {
                    enemyUnit = enemyUnits[std::stoi(owbw[2])];
                }
                else
                {
                    throw "Enemy unit with provided ID does not exists";
                }

                auto coordinates = enemyUnit->get_position();
                if (unit->attack(coordinates.first, coordinates.second))
                {
                    enemyUnit->damage(fight(unit->get_type(), enemyUnit->get_type()));

                    // if the attacked enemy's health is below or equal to 0, delete it
                    if (enemyUnit->get_health() <= 0)
                    {
                        delete enemyUnit;
                        enemyUnits.erase(enemyUnits.find(std::stoi(owbw[2])));
                    }
                }
                else
                {
                    throw "Given attack is not valid";
                }
            }
            else if (owbw[1] == "B")
            {
                base_ *base = static_cast<base_ *>(playerUnits[0]);
                if (base != unit)
                {
                    throw "Given ID is not a base";
                }

                if (base->get_time_remaining() <= 0)
                {
                    entity *unitTypeToBuild = create_entity(owbw[2].c_str()[0], 0, 0);
                    base->build(owbw[2].c_str()[0], unitTypeToBuild->get_build_time());
                    delete unitTypeToBuild;
                }
                else
                {
                    throw "Base is already building";
                }
            }
            else
            {
                throw "Given order is not valid";
            }
        }
    }
}

entity *mediator::create_entity(char unitType, int x, int y)
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
    }

    return newUnit;
}

int mediator::fight(char attacker, char defender)
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

bool mediator::can_move(int x, int y)
{
    // can't move to a square occupied by enemy unit
    auto &enemyUnits = (turn == 1) ? playerTwoUnits : playerOneUnits;
    for (auto enemyUnit : enemyUnits)
    {
        auto coordinates = enemyUnit.second->get_position();
        if (coordinates.first == x && coordinates.second == y)
        {
            return false;
        }
    }
    // can't move to an obstacle square
    for (auto obstacle : obstacles)
    {
        if (x == obstacle.first && y == obstacle.second)
            return false;
    }
    // can't exit the map
    if (x >= mapSize.first || y >= mapSize.second || x < 0 || y < 0)
        return false;
    // can't move to the enemy base square
    auto enemyBaseCoords = enemyUnits[0]->get_position();
    if (x == enemyBaseCoords.first && y == enemyBaseCoords.second)
        return false;

    return true;
}

void mediator::check_for_new_entities()
{
    auto &playerUnits = (turn) ? playerOneUnits : playerTwoUnits;
    auto &playerID = (turn) ? ID.first : ID.second;
    base_ *base = static_cast<base_ *>(playerUnits[0]);

    if(base->get_building_type() == '0')
        return;

    base->one_turn();

    if (base->get_time_remaining() <= 0)
    {
        std::pair<int, int> baseCoords = base->get_position();
        entity *newUnit = create_entity(base->get_building_type(), baseCoords.first, baseCoords.second);
        playerUnits.insert(std::make_pair(playerID, newUnit));
        
        playerID++;
        base->build('0', -1);
    }
}