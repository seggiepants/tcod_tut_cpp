#ifndef __CONFUSER_HPP__
#define __CONFUSER_HPP__

#include "Actor.hpp"
#include "Pickable.hpp"

namespace Game {
    class Confuser : public Pickable {
    public:
        Confuser(int countTurns, float range);
        bool use(Actor* owner, Actor* wearer);

        int countTurns;
        float range;
    };
}

#endif