#include "Healer.hpp"
#include "Actor.hpp"

Game::Healer::Healer(): amount(0.0f) {}
Game::Healer::Healer(float amount): amount(amount) {}

bool Game::Healer::use(std::shared_ptr<Actor> owner, Actor* wearer) {
    if (wearer->destructible) {
        float amountHealed = wearer->destructible->heal(amount);
        if (amountHealed > 0) {
            return Game::Pickable::use(owner, wearer);
        }
    }
    return false;
}

void Game::Healer::load(std::ifstream& stream) {
    char delim = ',';
    stream >> amount >> delim;
}

void Game::Healer::save(std::ofstream& stream) {
    const char delim = ',';
    stream << amount << delim;
}

