#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <libtcod.hpp>

namespace Game {
    struct Tile {
        bool explored; // has the player already seen this tile?
        Tile() : explored(false) {}
    };

    class Map {
    public:
        int width, height;    

        Map(int width, int height);
        ~Map();
        bool canWalk(int x, int y) const;
        bool isWall(int x, int y) const;
        bool isInFov(int x, int y) const;
        bool isExplored(int x, int y) const;
        void computeFov();
        void render() const;
    protected:
        Tile* tiles;
        TCODMap* map;
        friend class BspListener;
        void addItem(int x, int y);
        void addMonster(int x, int y);
        void dig(int x1, int y1, int x2, int y2);
        void createRoom(bool first, int x1, int y1, int x2, int y2);
        void setWall(int x, int y);
    };
}

#endif 