#include "Fireball.hpp"

#include "Actor.hpp"
#include "Engine.hpp"

Game::Fireball::Fireball() : range(0.0f), damage(0.0f) {}

Game::Fireball::Fireball(float range, float damage) : range(range), damage(damage) {}

bool Game::Fireball::use(Actor* owner, Actor* wearer) {
  engine.get_gui()->message(cyan, "Left-click a target tile for the fireball,\nor right-click to cancel.");
  engine.pickATile(this, owner, wearer, 0.0f);
  return true;
}

bool Game::Fireball::picked(Actor* owner, Actor* wearer, bool success, int x, int y) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  if (!success) {
    return false;
  }

  game->gui->message(orange, "The fireball explodes, burning everything within %g tiles", range);
  std::list<Actor*> damaged;
  for (auto const& actor : game->actors) {
    if (actor->destructible && !actor->destructible->isDead() && actor->getDistance(x, y) <= range) {
      damaged.push_back(actor);
    }
  }

  for (auto const& actor : damaged) {
    game->gui->message(orange, "The %s gets burned for %g hit points", actor->name.c_str(), damage);
    actor->destructible->takeDamage(actor, damage);
  }
  damaged.clear();

  return Pickable::use(owner, wearer);
}

void Game::Fireball::load(std::ifstream& stream) {
  char delim = ',';
  stream >> range >> delim >> damage >> delim;
}

void Game::Fireball::save(std::ofstream& stream) {
  const char delim = ',';
  stream << range << delim << damage << delim;
}
