#include "LightningBolt.hpp"
#include "Engine.hpp"

Game::LightningBolt::LightningBolt(float range, float damage) : range(range), damage(damage) {}

bool Game::LightningBolt::use(Actor* owner, Actor* wearer) {
    Actor* closestMonster = engine.getClosestMonster(wearer->x, wearer->y, range);

    if (!closestMonster) {
        engine.gui->message(lightGrey, "No enemy is close enough to strike.");
        return false;
    }

    // hit closest monster for <damage> hit points
    engine.gui->message(lightBlue, "A lightning bolt strikes the %s with a loud thunder!\nThe damage is %g hit points.", closestMonster->name->c_str(), damage);
    closestMonster->destructible->takeDamage(closestMonster, damage);
    return Pickable::use(owner, wearer);
}