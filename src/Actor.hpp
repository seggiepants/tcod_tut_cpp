#ifndef __ACTOR_HPP__
#define __ACTOR_HPP__

#include <libtcod.hpp>
#include <string>

class Actor {
    public:
    int x, y; // position on the map
    int ch; // ascii code
    std::string name;
    TCOD_ColorRGBA col; // color    

    Actor(int x, int y, int ch, const char* name, const TCOD_ColorRGBA & col);
    void render(tcod::Console& ) const;
    void update();
    bool moveOrAttack(int x, int y);
};

#endif //__ACTOR_HPP__