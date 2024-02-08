#include "MonsterDestructible.hpp"
#include <iostream> // remove when we have a gui log.
#include "Actor.hpp"

MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
    Destructible(maxHp,defense,corpseName) {
}

void MonsterDestructible::die(Actor* owner) {
    // Transform it into a nasty corpse! It doesn't block, can't be 
    // attacked and doesn't move.
    std::cout << owner->name->c_str() << " is dead" << std::endl;
    Destructible::die(owner);
}
