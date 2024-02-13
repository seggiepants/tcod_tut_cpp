#ifndef __CONTAINER_HPP__
#define __CONTAINER_HPP__

#include <list>
#include "Actor.hpp"

namespace Game {
    class Container {
    public:
        size_t size;
        std::list<Actor*> inventory;

        Container(size_t size);
        ~Container();
        bool add(Actor* actor);
        void remove(Actor* actor);
    };
}
#endif