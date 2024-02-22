#include "Attacker.hpp"

#include "Actor.hpp"
#include "Engine.hpp"

Game::Attacker::Attacker() : power(1.0f) {}
Game::Attacker::Attacker(float power) : power(power) {}

void Game::Attacker::attack(Actor* owner, Actor* target) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  TCODRandom* rnd = TCODRandom::getInstance();

  if (target->destructible != nullptr && !target->destructible->isDead()) {
    float adjustedDefense = roundf(rnd->getFloat(0.1f, target->destructible->defense, 0.75 * target->destructible->defense));
    float damage = power - adjustedDefense; // Don't make it so weak characters never hurt you.
    if (damage > 0) {
      game->gui->message(
          owner == game->player ? red : lightGrey,
          "%s attacks %s for %g hit points.",
          owner->name.c_str(),
          target->name.c_str(),
          damage);

    } else {
      game->gui->message(lightGrey, "%s attacks %s, but it has no effect!", owner->name.c_str(), target->name.c_str());
    }
    target->destructible->takeDamage(target, damage + target->destructible->defense); // Will be subtracting defense. Was power
  } else {
    game->gui->message(lightGrey, "%s attacks %s in vain.", owner->name.c_str(), target->name.c_str());
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
