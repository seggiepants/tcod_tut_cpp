#include "ConfusedMonsterAi.hpp"

#include <libtcod.hpp>

#include "Actor.hpp"
#include "Engine.hpp"

Game::ConfusedMonsterAi::ConfusedMonsterAi() : countTurns(1), oldAi(nullptr) {}
Game::ConfusedMonsterAi::ConfusedMonsterAi(int countTurns, Ai* oldAi) : countTurns(countTurns), oldAi(oldAi) {}

void Game::ConfusedMonsterAi::update(Actor* owner) {
  TCODRandom* rnd = TCODRandom::getInstance();
  Game* game = (Game*)engine.scenes[GameScene::GAME];

  // Reset the old AI and exit early if the actor is already dead.
  if (owner->destructible && owner->destructible->isDead()) {
    owner->ai = oldAi;
    delete this;
    return;
  }

  int dx = rnd->getInt(-1, 1);
  int dy = rnd->getInt(-1, 1);

  if (dx != 0 || dy != 0) {
    int destX = owner->x + dx;
    int destY = owner->y + dy;
    if (game->map->canWalk(destX, destY)) {
      owner->x = destX;
      owner->y = destY;
    } else {
      Actor* actor = engine.getActor(destX, destY);
      if (actor && owner->attacker) {
        owner->attacker->attack(owner, actor);
      }
    }
  }

  countTurns--;
  if (countTurns <= 0) {
    owner->ai = oldAi;
    delete this;
  }
}

void Game::ConfusedMonsterAi::load(std::ifstream& stream) {
  char delim = ',';
  bool flag = false;
  stream >> countTurns >> delim >> flag >> delim;
  if (flag) oldAi->load(stream);
}
void Game::ConfusedMonsterAi::save(std::ofstream& stream) {
  const char delim = ',';
  bool flag = oldAi != nullptr;
  stream << countTurns << delim;
  if (flag) oldAi->save(stream);
}
