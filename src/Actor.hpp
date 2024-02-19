#ifndef __ACTOR_HPP__
#define __ACTOR_HPP__

#include <iostream>
#include <fstream>
#include <libtcod.hpp>
#include <memory>
#include <string>

#include "Attacker.hpp"
#include "Destructible.hpp"
#include "Ai.hpp"
//#include "Pickable.hpp"
//#include "Healer.hpp"
//#include "LightningBolt.hpp"
//#include "Fireball.hpp"
//#include "Confuser.hpp"
//#include "Container.hpp"
//#include "MonsterDestructible.hpp"
//#include "PlayerDestructible.hpp"

namespace Game {
    class Confuser;
    class Container;
    class Fireball;
    class Healer;
    class LightningBolt;
    class MonsterDestructible;
    class Pickable;
    class PlayerDestructible;

    class Actor {
    public:
        int x, y; // position on the map
        int ch; // ascii code
        std::string name;
        TCOD_ColorRGBA col; // color    
        bool blocks; // can we walk on this actor?
        Attacker* attacker; // something that deals damage
        Destructible* destructible; // something that can be damaged
        Ai* ai; // something self-updating
        Pickable* pickable; // something that can be picked and used.
        Container* container; // something that can contain actors.
        Actor();
        Actor(int x, int y, int ch, const char* name, const TCOD_ColorRGBA & col);
        virtual ~Actor();
        void render() const;
        void update();
        void load(std::ifstream& stream);
        void save(std::ofstream& stream);
        float getDistance(int cx, int cy) const;
    };
}

#endif //__ACTOR_HPP__