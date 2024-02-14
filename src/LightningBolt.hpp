#ifndef __LIGHTNINGBOLT_HPP__
#define __LIGHTNINGBOLT_HPP__

#include "Actor.hpp"
#include "Pickable.hpp"

namespace Game {
    class LightningBolt : public Pickable {
    public:
        float range;
        float damage;
        LightningBolt(float range, float damage);
        bool use(Actor* owner, Actor* wearer);

    };
}

#endif