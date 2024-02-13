#include "Pickable.hpp"
#include "Engine.hpp"
#include "Container.hpp"

bool Game::Pickable::pick(Actor* owner, Actor* wearer){
    if (wearer->container && wearer->container->add(owner)) {
        engine.actors.remove(owner);
        return true;
    }
    return false;
}

bool Game::Pickable::use(Actor* owner, Actor* wearer) {
    if (wearer->container) {
        wearer->container->remove(owner);
        delete owner;
        return true;
    }
    return false;
}
