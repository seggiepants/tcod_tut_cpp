#ifndef __CONFUSER_HPP__
#define __CONFUSER_HPP__

#include <iostream>
#include <fstream>
#include "Pickable.hpp"

namespace Game {
    class Actor;

    class Confuser : public Pickable {
    public:
        Confuser();
        Confuser(int countTurns, float range);
        bool use(Actor* owner, Actor* wearer);
        bool picked(Actor* owner, Actor* wearer, bool success, int x, int y);
        int getType() {return (int)PickableType::CONFUSER; };
        void load(std::ifstream& stream);
        void save(std::ofstream& stream);
        int countTurns;
        float range;
    };
}

#endif
