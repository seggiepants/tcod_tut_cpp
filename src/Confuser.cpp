#include "Confuser.hpp"
#include "ConfusedMonsterAi.hpp"
#include "Engine.hpp"

Game::Confuser::Confuser(int countTurns, float range) : countTurns(countTurns), range(range) {}

bool Game::Confuser::use(Actor* owner, Actor* wearer) {
    engine.gui->message(cyan, "Left-click an enemy to confuse it,\nor right-click to cancel.");
    int x, y;
    if (!engine.pickATile(&x, &y, range)) {
        return false;
    }

    Actor* actor = engine.getActor(x, y);
    if (!actor) {
        return false;
    }

    // Confuse the monster for <countTurns> turns
    Ai* confusedAi = new ConfusedMonsterAi(countTurns, actor->ai);
    actor->ai = confusedAi;

    engine.gui->message(lightGreen, "The eyes of the %s look vacant,\nas they start to stumble around!", actor->name->c_str());
    return Pickable::use(owner, wearer);
}