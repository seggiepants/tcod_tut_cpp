#include "Pickable.hpp"
#include "Engine.hpp"
#include "Container.hpp"
#include "Healer.hpp"
#include "LightningBolt.hpp"
#include "Fireball.hpp"
#include "Confuser.hpp"

bool Game::Pickable::pick(std::shared_ptr<Actor> owner, Actor* wearer){
    if (wearer->container && wearer->container->add(owner)) {
        engine.actors->remove(owner);
        return true;
    }
    return false;
}
void Game::Pickable::drop(std::shared_ptr<Actor> owner, Actor* wearer) {
    if (wearer->container && wearer->container.get()) {
        wearer->container->remove(owner);
        
        engine.actors->push_back(owner);
        engine.sendToBack(owner);

        owner->x = wearer->x;
        owner->y = wearer->y;
        engine.gui->message(lightGrey, "%s drops a %s", wearer->name.c_str(), owner->name.c_str());
    }
}

std::shared_ptr<Game::Pickable> Game::Pickable::create(int type) {
    std::shared_ptr<Pickable> pickable = nullptr;
    switch((PickableType)type) {
        case HEALER : pickable = std::make_shared<Healer>(0); break;
        case LIGHTNING_BOLT : pickable = std::make_shared<LightningBolt>(0,0); break;
        case CONFUSER : pickable = std::make_shared<Confuser>(0,0); break;
        case FIREBALL : pickable = std::make_shared<Fireball>(0,0); break;
    }
    return pickable;
}
bool Game::Pickable::use(std::shared_ptr<Actor> owner, Actor* wearer) {
    if (wearer->container && wearer->container.get()) {
        wearer->container->remove(owner);
        owner.reset();
        return true;
    }
    return false;
}


