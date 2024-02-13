#ifndef __PICKABLE_HPP__
#define __PICKABLE_HPP__

#include "Actor.hpp"

namespace Game {
    class Pickable {
    public:
        virtual ~Pickable() {};
        bool pick(Actor* owner, Actor* wearer);
        virtual bool use(Actor* owner, Actor* wearer);
    };
}

#endif