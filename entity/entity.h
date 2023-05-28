#ifndef ENTITY_H
#define ENTITY_H

#include <utility>
#include "cmath"

class entity
{
public:
    entity(){};

    // checks if the entity can move to the given coordinates
    // it does not check if the coordinates are occupied or even exist
    bool move(int newX, int newY);
    
    // checks if the entity has the range to attack given coordinates
    // it does not check if the coordinates are occupied by the enemy entity
    bool attack(int attX, int attY);

    // subtracts given int from current health
    void damage(int dmg);

    // returns current coordinates: x and y
    std::pair<int, int> get_position();

    char get_type() { return unitType; }
    int get_health() { return health; }
    int get_build_time() { return buildTime; }
    void set_health(int newHp) { health = newHp; };
    int get_speed() { return currentSpeed; };
    bool attacked_() { return attacked; };
    int get_cost() { return cost; };

    // sets attacked back to false, and currentSpeed to speed
    void next_turn();

protected:
    // current position
    int x, y;
    int health;
    int speed;
    int currentSpeed;
    int cost;
    int attackRange;
    int buildTime;
    char unitType;
    // only one attack per turn is allowed
    bool attacked;
};

#endif