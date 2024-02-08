#include "PlayerDestructible.hpp"
#include "Engine.hpp"
#include <iostream> // delete later when cout calls are removed

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
    Destructible(maxHp,defense,corpseName) {
}

void PlayerDestructible::die(Actor* owner) {
    std::cout << "You died!" << std::endl;
    Destructible::die(owner);
    engine.gameStatus = Engine::DEFEAT;
}