#include "Destructible.hpp"
#include "Actor.hpp"
#include "Engine.hpp"
#include <string>

Game::Destructible::Destructible(float maxHp, float defense, const char* corpseName) :
    maxHp(maxHp),
    hp(maxHp),
    defense(defense)
{
    this->corpseName = new std::string(corpseName);
}

Game::Destructible::~Destructible() {
    delete corpseName;
}

float Game::Destructible::heal(float amount) {
    hp += amount;
    if (hp > maxHp) {
        amount -= hp - maxHp;
        hp = maxHp;
    }
    return amount;
}

float Game::Destructible::takeDamage(Actor* owner, float damage) {
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

void Game::Destructible::die(Actor* owner) {    
    // transform the actor into a corpse!
    owner->ch = '%';
    owner->col = COLOR_CORPSE;
    delete owner->name;
    owner->name = new std::string(corpseName->c_str());
    owner->blocks = false;
    // Make sure corpses are drawn before living actors
    engine.sendToBack(owner);

}