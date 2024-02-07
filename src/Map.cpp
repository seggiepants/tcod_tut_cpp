#include "Map.hpp"
#include "Engine.hpp"

// Max size not used yet.
// static const int ROOM_MAX_SIZE = 12;
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

#ifdef _WIN32
#define UNUSED
#else
#define UNUSED __attribute__((unused))
#endif

bool BspListener::visitNode(TCODBsp* node, UNUSED void* userData) {
    if (node->isLeaf()) {
        int x, y, w, h;

        // dig a room
        TCODRandom* rng = TCODRandom::getInstance();
        w = rng->getInt(ROOM_MIN_SIZE, node->w - 2);
        h = rng->getInt(ROOM_MIN_SIZE, node->h - 2);
        x = rng->getInt(node->x + 1, node->x + node->w -  w - 1);
        y = rng->getInt(node->y + 1, node->y + node->h -  h - 1);
        map.createRoom(this->roomNum == 0, x, y, x + w - 1, y + h - 1);

        if (roomNum != 0) {
            // dig a corridor from the last room
            map.dig(this->lastX, this->lastY, x + (w / 2), this->lastY);
            map.dig(x + (w / 2), this->lastY, x + (w / 2), y + (h / 2));            
        }
        this->lastX = x + (w / 2);
        this->lastY = y + (h / 2);
        this->roomNum++;
    }    
    return true;
}

Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width * height];
    map = new TCODMap(width, height);
    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(NULL, 8, ROOM_MIN_SIZE + 2, ROOM_MIN_SIZE + 2, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
}

Map::~Map() {
    delete[] tiles;
    delete map;
}

bool Map::isWall(int x, int y) const {
    return !map->isWalkable(x, y);
}

bool Map::isExplored(int x, int y) const {
    return tiles[x + (y * width)].explored;
}

bool Map::isInFov(int x, int y) const {
    if (map->isInFov(x, y)) {
        tiles[x + (y * width)].explored = true;
        return true;
    }
    return false;
}

void Map::computeFov() {
    map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
}

void Map::setWall(int x, int y) {
    map->setProperties(x, y, true, false);    
}

void Map::render(tcod::Console & console) const {
    static const TCOD_ColorRGBA unexplored {0, 0, 0, 255};
    static const TCOD_ColorRGBA darkWall {0, 0, 100, 255};
    static const TCOD_ColorRGBA darkGround {50, 50, 150, 255};    
    static const TCOD_ColorRGBA lightWall {130, 110, 50, 255 };
    static const TCOD_ColorRGBA lightGround {200, 180, 50, 255 };


    for(int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            TCOD_ConsoleTile& tile = console.at(x, y);
            tile.ch = ' ';
            if (isInFov(x, y)) {
                tile.bg = isWall(x, y) ? lightWall : lightGround;
            } else if (isExplored(x, y)) {
                tile.bg = isWall(x, y) ? darkWall : darkGround;
            }
            else
            {
                tile.bg = unexplored;
            }
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
            map->setProperties(x, y, true, true);
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