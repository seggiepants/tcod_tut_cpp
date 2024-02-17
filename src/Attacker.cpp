#include "Attacker.hpp"
#include "Actor.hpp"
#include "Engine.hpp"

Game::Attacker::Attacker() : power(1.0f) {}
Game::Attacker::Attacker(float power) : power(power) {}

void Game::Attacker::attack(Actor* owner, Actor* target) {
    if (target->destructible != nullptr && !target->destructible->isDead()) {
        float damage = power - target->destructible->defense;
        if (damage > 0) {
            engine.gui->message(owner == engine.player.get() ? red : lightGrey, "%s attacks %s for %g hit points." , owner->name.c_str(), target->name.c_str(), damage);

        } else {
            engine.gui->message(lightGrey, "%s attacks s, but it has no effect!", owner->name.c_str(), target->name.c_str());            
        }
        target->destructible->takeDamage(target, power);
    } else {
        engine.gui->message(lightGrey, "%s attacks %s in vain.", owner->name.c_str(), target->name.c_str());
    }
}

void Game::Attacker::load(std::ifstream& stream) {
    char delim = ',';
    stream >> power >> delim;
}

void Game::Attacker::save(std::ofstream& stream) {
    const char delim = ',';
    stream << power << delim;
}