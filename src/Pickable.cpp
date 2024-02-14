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
void Game::Pickable::drop(Actor* owner, Actor* wearer) {
    if (wearer->container) {
        wearer->container->remove(owner);
        engine.actors.push_back(owner);
        engine.sendToBack(owner);
        owner->x = wearer->x;
        owner->y = wearer->y;
        engine.gui->message(lightGrey, "%s drops a %s", wearer->name->c_str(), owner->name->c_str());
    }
}

bool Game::Pickable::use(Actor* owner, Actor* wearer) {
    if (wearer->container) {
        wearer->container->remove(owner);
        delete owner;
        return true;
    }
    return false;
}
