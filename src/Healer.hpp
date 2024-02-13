#ifndef __HEALER_HPP__
#define __HEALER_HPP__

#include "Pickable.hpp"

class Healer : public Game::Pickable {
public:
    float amount; // how many hp

    Healer(float amouont);
    bool use(Actor* owner, Actor* wearer);
};

#endif