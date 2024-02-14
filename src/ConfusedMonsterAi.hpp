#ifndef __CONFUSED_MONSTER_AI_HPP__
#define __CONFUSED_MONSTER_AI_HPP__

#include "Ai.hpp"
#include "Actor.hpp"

namespace Game {

    class ConfusedMonsterAi : public Ai {
    public:
        ConfusedMonsterAi(int countTurns, Ai* oldAi);
        void update(Actor* owner);
    protected:
        int countTurns;
        Ai* oldAi;
    };
}

#endif