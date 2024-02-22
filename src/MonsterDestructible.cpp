#include "MonsterDestructible.hpp"

#include "Actor.hpp"
#include "Engine.hpp"

Game::MonsterDestructible::MonsterDestructible() : Destructible(0.0f, 0.0f, nullptr, 0) {}

Game::MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char* corpseName, int xp)
    : Destructible(maxHp, defense, corpseName, xp) {}

void Game::MonsterDestructible::die(Actor* owner) {
  // Transform it into a nasty corpse! It doesn't block, can't be
  // attacked and doesn't move.
  engine.get_gui()->message(lightGrey, "%s is dead, you gain %d xp", owner->name.c_str(), xp);
  ((Game*)engine.scenes[GameScene::GAME])->player->destructible->xp += xp;  
  Destructible::die(owner);
}

void Game::MonsterDestructible::save(std::ofstream& stream) {
  // Write out the type then let the parent save continue.
  Destructible::save(stream);
}
