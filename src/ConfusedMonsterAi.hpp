#ifndef __CONFUSED_MONSTER_AI_HPP__
#define __CONFUSED_MONSTER_AI_HPP__

#include <iostream>
#include <fstream>
#include <memory>
#include "Ai.hpp"

namespace Game {

    class Actor;

    class ConfusedMonsterAi : public Ai {
    public:
        ConfusedMonsterAi();
        ConfusedMonsterAi(int countTurns, Ai* oldAi);
        void update(Actor* owner);
        int getType() {return (int)AiType::CONFUSED_MONSTER; };
        void load(std::ifstream& stream);
        void save(std::ofstream& stream);
        template <class Archive> void serialize(Archive& ar) {
            ar(countTurns, oldAi); 
        }
    protected:
        int countTurns;
        Ai* oldAi;
    };
}

#endif