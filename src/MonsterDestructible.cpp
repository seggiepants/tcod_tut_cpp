#include "MonsterDestructible.hpp"
#include "Actor.hpp"
#include "Engine.hpp"

Game::MonsterDestructible::MonsterDestructible() :
    Destructible(0.0f, 0.0f, nullptr) {}

Game::MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
    Destructible(maxHp,defense,corpseName) {
}

void Game::MonsterDestructible::die(Actor* owner) {
    // Transform it into a nasty corpse! It doesn't block, can't be 
    // attacked and doesn't move.
    engine.gui->message(lightGrey, "%s is dead", owner->name.c_str());
    Destructible::die(owner);
}

void Game::MonsterDestructible::save(std::ofstream& stream) {
    // Write out the type then let the parent save continue.
    Destructible::save(stream);    
}
