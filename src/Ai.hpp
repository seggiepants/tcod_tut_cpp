#ifndef __AI_HPP__
#define __AI_HPP__

#include <iostream>
#include <fstream>
#include <memory>

namespace Game {

    class Actor;

    class Ai {
    public:
        virtual ~Ai() {};
        virtual void update(Actor* owner) = 0; // Must override
        virtual int getType() = 0; // Must override
        static Ai* create(int type);
        virtual void load(std::ifstream& stream) = 0;
        virtual void save(std::ofstream& stream) = 0;
    //protected:
        enum AiType {
            MONSTER,CONFUSED_MONSTER,PLAYER
        };
    };
}

#endif