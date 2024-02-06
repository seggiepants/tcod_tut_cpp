#include "Map.hpp"

Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width * height];
    setWall(30, 22);
    setWall(50, 22);
}

Map::~Map() {
    delete[] tiles;
    tiles = nullptr;
}

bool Map::isWall(int x, int y) const {
    return !tiles[x + (y * width)].canWalk;
}

void Map::setWall(int x, int y) {
    tiles[x + (y * width)].canWalk = false;
}

void Map::render(tcod::Console & console) const {
    static const TCOD_ColorRGBA darkWall {0, 0, 100, 255};
    static const TCOD_ColorRGBA darkGround {50, 50, 150, 255};    


    for(int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            TCOD_ConsoleTile& tile = console.at(x, y);
            tile.ch = ' ';
            tile.bg = isWall(x, y) ? darkWall : darkGround;
        }
    }
}
