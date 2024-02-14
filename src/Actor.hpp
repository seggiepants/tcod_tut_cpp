#ifndef __ACTOR_HPP__
#define __ACTOR_HPP__

#include <libtcod.hpp>
#include <string>

#include "Attacker.hpp"
#include "Destructible.hpp"
#include "Ai.hpp"

namespace Game { 
    class Container; 
    class Pickable;
}

namespace Game {
    class Actor {
        public:
        int x, y; // position on the map
        int ch; // ascii code
        std::string* name;
        TCOD_ColorRGBA col; // color    
        bool blocks; // can we walk on this actor?
        Attacker* attacker; // something that deals damage
        Destructible* destructible; // something that can be damaged
        Ai* ai; // something self-updating
        Game::Pickable* pickable; // something that can be picked and used.
        Game::Container* container; // something that can contain actors.

        Actor(int x, int y, int ch, const char* name, const TCOD_ColorRGBA & col);
        ~Actor();
        void render() const;
        void update();
        float getDistance(int cx, int cy) const;
    };
}

#endif //__ACTOR_HPP__