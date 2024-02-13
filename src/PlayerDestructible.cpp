#include "PlayerDestructible.hpp"
#include "Engine.hpp"

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
    Destructible(maxHp,defense,corpseName) {
}

void PlayerDestructible::die(Actor* owner) {
    engine.gui->message(red, "You died!");
    Destructible::die(owner);
    engine.gameStatus = Engine::DEFEAT;
}