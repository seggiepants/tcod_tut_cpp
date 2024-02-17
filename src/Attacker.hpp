#ifndef __ATTACKER_HPP__
#define __ATTACKER_HPP__

#include <iostream>
#include <fstream>
#include <memory>

namespace Game {
    class Actor;

    class Attacker {
    public:
        float power; // hit points given
        Attacker();
        Attacker(float power);
        void attack(Actor* owner, Actor* target);
        void load(std::ifstream& stream);
        void save(std::ofstream& stream);
    };
}
#endif