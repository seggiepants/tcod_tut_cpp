#ifndef __PLAYERAI_HPP__
#define __PLAYERAI_HPP__

#include "Ai.hpp"

class PlayerAi : public Ai {
    public:
    void update(Actor* owner);

    protected:
    bool moveOrAttack(Actor* owner, int targetX, int targetY);
};

#endif