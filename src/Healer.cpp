#include "Healer.hpp"

Game::Healer::Healer(float amount): amount(amount) {}

bool Game::Healer::use(Actor* owner, Actor* wearer) {
    if (wearer->destructible) {
        float amountHealed = wearer->destructible->heal(amount);
        if (amountHealed > 0) {
            return Game::Pickable::use(owner, wearer);
        }
    }
    return false;
}
