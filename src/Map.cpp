#include "Map.hpp"
#include "Engine.hpp"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;

class BspListener : public ITCODBspCallback { 
    private:
    Map &map; // a map to dig
    int roomNum; // room number
    int lastX, lastY; // center of the last room;
    public :
    BspListener(Map &map) : map(map), roomNum(0) {};
    bool visitNode(TCODBsp* node, void* userData);
};

bool BspListener::visitNode(TCODBsp* node, __attribute__((unused)) void* userData) {
    if (node->isLeaf()) {
        int x, y, w, h;

        // dig a room
        TCODRandom* rng = TCODRandom::getInstance();
        w = rng->getInt(ROOM_MIN_SIZE, node->w - 2);
        h = rng->getInt(ROOM_MIN_SIZE, node->h - 2);
        x = rng->getInt(node->x + 1, node->x + node->w -  w - 1);
        y = rng->getInt(node->y + 1, node->y + node->h -  h - 1);
        map.createRoom(roomNum == 0, x, y, x + w - 1, y + h - 1);

        if (roomNum != 0) {
            // dig a corridor from the last room
            map.dig(lastX, lastY, x + (w / 2), lastY);
            map.dig(x + (w / 2), lastY, x + (w / 2), y + (h / 2));            
        }
        lastX = x + (w / 2);
        lastY = y + (h / 2);
        roomNum++;
    }    
    return true;
}

Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width * height];
    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(NULL, 8, ROOM_MIN_SIZE + 2, ROOM_MIN_SIZE + 2, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
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

void Map::dig(int x1, int y1, int x2, int y2) {
    if (x2 < x1) { 
        std::swap(x1, x2);
    }
    if (y2 < y1) {
        std::swap(y1, y2);
    }

    for (int y = y1; y <= y2; ++y) {
        for (int x = x1; x <= x2; ++x) {
            tiles[x + (y * width)].canWalk = true;
        }
    }
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2) {
    dig(x1, y1, x2, y2);
    if (first) {
        engine.player->x = (x1 + x2) / 2;
        engine.player->y = (y1 + y2) / 2;        
    } else {
        TCODRandom* rng = TCODRandom::getInstance();
        if (rng->getInt(0, 3) == 0) {
            engine.actors.push_back(new Actor((x1 + x2) /2, (y1 + y2) / 2, '@', YELLOW));
        }
    }
}