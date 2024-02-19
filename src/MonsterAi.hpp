#ifndef __MONSTERAI_HPP__
#define __MONSTERAI_HPP__

#include "Ai.hpp"

namespace Game {
    class Actor;
    
    class MonsterAi : public Ai {
    public:
        ~MonsterAi() {}
        void update(Actor* owner);   
        int getType() {return (int)AiType::MONSTER; };
        void load(std::ifstream& stream);
        void save(std::ofstream& stream);
    protected:
        void moveOrAttack(Actor* owner, int targetX, int targetY);
        int moveCount = 0;
    };
}

#endif