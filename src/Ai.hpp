#ifndef __AI_HPP__
#define __AI_HPP__

class Actor;

class Ai {
    public:
    virtual void update(Actor* owner) = 0; // Must override
};

#endif