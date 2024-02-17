#ifndef __FIREBALL_HPP__
#define __FIREBALL_HPP__

#include <iostream>
#include <fstream>
#include "Pickable.hpp"

namespace Game {

    class Actor;

    class Fireball : public Pickable {
    public:
        float range;
        float damage;
        Fireball();
        Fireball(float range, float damage);
        bool use(std::shared_ptr<Actor> owner, Actor* wearer);
        int getType() {return (int)Game::Pickable::PickableType::FIREBALL; };
        void load(std::ifstream& stream);
        void save(std::ofstream& stream);
    };

}

#endif