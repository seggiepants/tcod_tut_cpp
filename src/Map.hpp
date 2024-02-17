#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <iostream>
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
        virtual ~Map();
        void init(bool withActors);
        bool canWalk(int x, int y) const;
        bool isWall(int x, int y) const;
        bool isInFov(int x, int y) const;
        bool isExplored(int x, int y) const;
        void computeFov();
        void render() const;

        void load(std::ifstream& stream);
        void save(std::ofstream& stream);

    protected:
        Tile* tiles;
        TCODMap* map;
        long seed;
        TCODRandom* rng;
        friend class BspListener;
        void addItem(int x, int y);
        void addMonster(int x, int y);
        void dig(int x1, int y1, int x2, int y2);
        void createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors);
        void setWall(int x, int y);
    };
}

#endif 