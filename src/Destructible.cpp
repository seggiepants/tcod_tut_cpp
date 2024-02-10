#include "Destructible.hpp"
#include "Actor.hpp"
#include "Engine.hpp"
#include <string>

Destructible::Destructible(float maxHp, float defense, const char* corpseName) :
    maxHp(maxHp),
    hp(maxHp),
    defense(defense)
{
    this->corpseName = new std::string(corpseName);
}

Destructible::~Destructible() {
    delete corpseName;
}

float Destructible::takeDamage(Actor* owner, float damage) {
    damage -= defense;
    if (damage > 0) {
        hp -= damage;
        if (hp <= 0) {
            die(owner);
        }
    } else {
        damage = 0;
    }
    return damage;
}

void Destructible::die(Actor* owner) {    
    // transform the actor into a corpse!
    owner->ch = '%';
    owner->col = COLOR_CORPSE;
    delete owner->name;
    owner->name = new std::string(corpseName->c_str());
    owner->blocks = false;
    // Make sure corpses are drawn before living actors
    engine.sendToBack(owner);

}