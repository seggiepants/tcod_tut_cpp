#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <libtcod.hpp>

struct Tile {
    bool canWalk; // can we walk thorugh this tile?
    Tile() : canWalk(false) {}
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
        friend class BspListener;

        void dig(int x1, int y1, int x2, int y2);
        void createRoom(bool first, int x1, int y1, int x2, int y2);
        void setWall(int x, int y);        
};

#endif 