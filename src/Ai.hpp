#ifndef __AI_HPP__
#define __AI_HPP__

namespace Game {

    class Actor;

    class Ai {
        public:
        virtual ~Ai() {};
        virtual void update(Actor* owner) = 0; // Must override
    };
}

#endif