#ifndef __CONTAINER_HPP__
#define __CONTAINER_HPP__

#include <iostream>
#include <fstream>
#include <list>
#include <memory>
#include <libtcod.hpp>

namespace Game {

    class Actor;

    class Container {
    public:
        size_t size;
        std::list<std::shared_ptr<Actor>> inventory;

        Container();
        Container(size_t size);
        virtual ~Container();
        bool add(std::shared_ptr<Actor> actor);
        void remove(std::shared_ptr<Actor> actor);
        void load(std::ifstream& stream);
        void save(std::ofstream& stream);
    };
}
#endif