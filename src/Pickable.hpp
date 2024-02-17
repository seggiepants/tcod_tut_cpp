#ifndef __PICKABLE_HPP__
#define __PICKABLE_HPP__

#include <iostream>
#include <fstream>
#include <memory>
// #include "Actor.hpp"

namespace Game {
    class Actor;
    
    class Pickable {
    public:
        virtual ~Pickable() {};
        bool pick(std::shared_ptr<Actor> owner, Actor* wearer);
        void drop(std::shared_ptr<Actor> owner, Actor* wearer);
        virtual bool use(std::shared_ptr<Actor> owner, Actor* wearer);
        virtual int getType() = 0; // Must override
        virtual void load(std::ifstream& stream) = 0;
        virtual void save(std::ofstream& stream) = 0;
        static std::shared_ptr<Pickable> create(int type);
    protected:
        enum PickableType {
            HEALER,LIGHTNING_BOLT,CONFUSER,FIREBALL
        };
    };
}

#endif