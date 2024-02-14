#include "Fireball.hpp"
#include "Engine.hpp"

Game::Fireball::Fireball(float range, float damage) : LightningBolt(range, damage) {}

bool Game::Fireball::use(Actor* owner, Actor* wearer) {
    engine.gui->message(cyan, "Left-click a target tile for the fireball,\nor right-click to cancel.");
    int x, y;
    if (!engine.pickATile(&x, &y)) {
        return false;
    }

    engine.gui->message(orange, "The fireball explodes, burning everything within %g tiles", range);
    std::list<Actor*> damaged;
    for(auto const & actor : engine.actors) {
        if (actor->destructible && !actor->destructible->isDead() && actor->getDistance(x, y) <= range) {
            damaged.push_back(actor);
        }
    }

    for(auto const & actor : damaged) {
        engine.gui->message(orange, "The %s gets burned for %g hit points", actor->name->c_str(), damage);
        actor->destructible->takeDamage(actor, damage);
    }
    damaged.clear();

    return Pickable::use(owner, wearer);
}