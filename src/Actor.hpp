#ifndef __ACTOR_HPP__
#define __ACTOR_HPP__

#include <libtcod.hpp>

class Actor {
    public:
    int x, y; // position on the map
    int ch; // ascii code
    TCOD_ColorRGBA col; // color    

    Actor(int x, int y, int ch, const TCOD_ColorRGBA & col);
    void render(tcod::Console& ) const;
};

#endif //__ACTOR_HPP__