#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <libtcod.hpp>

struct Tile {
    bool canWalk; // can we walk thorugh this tile?
    Tile() : canWalk(true) {}
};

class Map {
    public:
        int width, height;

        Map(int width, int height);
        ~Map();
        bool isWall(int x, int y) const;
        void render(tcod::Console&) const;
    protected:
        Tile* tiles;
        void setWall(int x, int y);        
};

#endif 