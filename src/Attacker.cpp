#include "Attacker.hpp"
#include "Actor.hpp"
#include <iostream> // should remove once we have a proper console/log gui

Attacker::Attacker(float power) : power(power) {}

void Attacker::attack(Actor* owner, Actor* target) {
    if (target->destructible && !target->destructible->isDead()) {
        float damage = power - target->destructible->defense;
        if (damage > 0) {
            std::cout << owner->name->c_str() << " attacks " << target->name->c_str() << " for " << damage << " hit points." << std::endl;
        } else {
            std::cout << owner->name->c_str() << " attacks " << target->name->c_str() << ", but it has no effect!" << std::endl;
        }
        target->destructible->takeDamage(target, power);
    } else {
        std::cout << owner->name->c_str() << " attacks " << target->name->c_str() << " in vain." << std::endl;
    }
}