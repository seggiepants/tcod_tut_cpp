#include "Confuser.hpp"
#include "ConfusedMonsterAi.hpp"
#include "Engine.hpp"
#include "Actor.hpp"

Game::Confuser::Confuser() : countTurns(1), range(1.0f) {}
Game::Confuser::Confuser(int countTurns, float range) : countTurns(countTurns), range(range) {}

bool Game::Confuser::use(std::shared_ptr<Actor> owner, Actor* wearer) {
    engine.gui->message(cyan, "Left-click an enemy to confuse it,\nor right-click to cancel.");
    int x, y;
    if (!engine.pickATile(&x, &y, range)) {
        return false;
    }

    std::shared_ptr<Actor> actor = engine.getActor(x, y);
    if (!actor) {
        return false;
    }

    // Confuse the monster for <countTurns> turns
    Ai* confusedAi = new ConfusedMonsterAi(countTurns, actor->ai);
    actor->ai = confusedAi;

    engine.gui->message(lightGreen, "The eyes of the %s look vacant,\nas they start to stumble around!", actor->name.c_str());
    return Pickable::use(owner, wearer);
}

void Game::Confuser::load(std::ifstream& stream) {
    char delim = ',';
    stream >> countTurns >> delim >> range >> delim;
}

void Game::Confuser::save(std::ofstream& stream) {
    const char delim = ',';
    stream << countTurns << delim << range << delim;
}
        

