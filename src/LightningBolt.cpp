#include "LightningBolt.hpp"

#include "Actor.hpp"
#include "Engine.hpp"

Game::LightningBolt::LightningBolt() : range(0.0f), damage(0.0f) {}

Game::LightningBolt::LightningBolt(float range, float damage) : range(range), damage(damage) {}

bool Game::LightningBolt::use(Actor* owner, Actor* wearer) {
  Actor* closestMonster = engine.getClosestMonster(wearer->x, wearer->y, range);

  if (!closestMonster) {
    engine.get_gui()->message(lightGrey, "No enemy is close enough to strike.");
    return false;
  }

  // hit closest monster for <damage> hit points
  engine.get_gui()->message(
      lightBlue,
      "A lightning bolt strikes the %s with a loud thunder!\nThe damage is %g hit points.",
      closestMonster->name.c_str(),
      damage);
  closestMonster->destructible->takeDamage(closestMonster, damage);
  return Pickable::use(owner, wearer);
}

void Game::LightningBolt::load(std::ifstream& stream) {
  char delim = ',';
  stream >> range >> delim >> damage >> delim;
}

void Game::LightningBolt::save(std::ofstream& stream) {
  const char delim = ',';
  stream << range << delim << damage << delim;
}
