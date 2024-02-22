#include "Ai.hpp"

#include "ConfusedMonsterAi.hpp"
#include "MonsterAi.hpp"
#include "PlayerAi.hpp"

Game::Ai* Game::Ai::create(int type) {
  Ai* ai = nullptr;
  switch ((AiType)type) {
    case PLAYER:
      ai = new PlayerAi();
      break;
    case MONSTER:
      ai = new MonsterAi();
      break;
    case CONFUSED_MONSTER:
      new ConfusedMonsterAi(0, nullptr);
      break;
  }
  return ai;
}
