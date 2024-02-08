#ifndef __PLAYERDESTRUCTIBLE_HPP__
#define __PLAYERDESTRUCTIBLE_HPP__

#include "Destructible.hpp"

class PlayerDestructible : public Destructible {
    public:
    PlayerDestructible(float maxHp, float defense, const char* corpseName);
    void die(Actor* owner);
};

#endif