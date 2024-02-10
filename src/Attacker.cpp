#include "Attacker.hpp"
#include "Actor.hpp"
#include "Engine.hpp"

Attacker::Attacker(float power) : power(power) {}

void Attacker::attack(Actor* owner, Actor* target) {
    if (target->destructible && !target->destructible->isDead()) {
        float damage = power - target->destructible->defense;
        if (damage > 0) {
            engine.gui->message(owner == engine.player ? red : lightGrey, "%s attacks %s for %g hit points." , owner->name->c_str(), target->name->c_str(), damage);

        } else {
            engine.gui->message(lightGrey, "%s attacks s, but it has no effect!", owner->name->c_str(), target->name->c_str());            
        }
        target->destructible->takeDamage(target, power);
    } else {
        engine.gui->message(lightGrey, "%s attacks %s in vain.", owner->name->c_str(), target->name->c_str());
    }
}