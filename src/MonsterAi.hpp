#ifndef __MONSTERAI_HPP__
#define __MONSTERAI_HPP__

#include "Ai.hpp"

namespace Game {
    class MonsterAi : public Ai {
    public:
        void update(Actor* owner);

    protected:
        void moveOrAttack(Actor* owner, int targetX, int targetY);
        int moveCount = 0;
    };
}

#endif