#include "Ai.hpp"
#include "PlayerAi.hpp"
#include "MonsterAi.hpp"
#include "ConfusedMonsterAi.hpp"

Game::Ai* Game::Ai::create(int type) {
    Ai* ai = nullptr;
    switch((AiType)type) {
        case PLAYER : ai = new PlayerAi(); break;
        case MONSTER : ai = new MonsterAi(); break;
        case CONFUSED_MONSTER : new ConfusedMonsterAi(0, nullptr); break;
    }
    return ai;
}
