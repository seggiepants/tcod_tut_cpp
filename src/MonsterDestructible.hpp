#ifndef __MONSTERDESTRUCTIBLE_HPP__
#define __MONSTERDESTRUCTIBLE_HPP__

#include "Destructible.hpp"

class MonsterDestructible : public Destructible {
    public:
    MonsterDestructible(float maxHp, float defense, const char* corpseName);
    void die(Actor* owner);
};

#endif