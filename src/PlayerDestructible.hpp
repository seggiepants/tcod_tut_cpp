#ifndef __PLAYERDESTRUCTIBLE_HPP__
#define __PLAYERDESTRUCTIBLE_HPP__

#include <iostream>
#include <fstream>
#include "Destructible.hpp"

namespace Game {
    class PlayerDestructible : public Destructible {
    public:
        PlayerDestructible(float maxHp, float defense, const char* corpseName);
        ~PlayerDestructible() { };
        int getType() { return (int) DestructibleType::PLAYER; }
        void die(Actor* owner);
        void save(std::ofstream& stream);
    };
}

#endif