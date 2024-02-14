#include "ConfusedMonsterAi.hpp"
#include "Engine.hpp"
#include <libtcod.hpp>

Game::ConfusedMonsterAi::ConfusedMonsterAi(int countTurns, Ai* oldAi) : countTurns(countTurns), oldAi(oldAi) {}

void Game::ConfusedMonsterAi::update(Actor* owner) {
    TCODRandom* rnd = TCODRandom::getInstance();
    int dx = rnd->getInt(-1, 1);
    int dy = rnd->getInt(-1, 1);

    if (dx != 0 || dy != 0) {
        int destX = owner->x + dx;
        int destY = owner->y + dy;
        if (engine.map->canWalk(destX, destY)) {
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