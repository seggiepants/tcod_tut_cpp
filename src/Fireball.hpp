#ifndef __FIREBALL_HPP__
#define __FIREBALL_HPP__

#include "LightningBolt.hpp"

namespace Game {

    class Fireball : public LightningBolt {
    public:
        Fireball(float range, float damage);
        bool use(Actor* owner, Actor* wearer);
    };

}

#endif