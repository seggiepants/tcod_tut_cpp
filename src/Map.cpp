#include "Map.hpp"
#include "Engine.hpp"
#include "Attacker.hpp"
#include "Confuser.hpp"
#include "Fireball.hpp"
#include "Healer.hpp"
#include "LightningBolt.hpp"
#include "MonsterAi.hpp"
#include "MonsterDestructible.hpp"

// Max size not used yet.
static const int ROOM_MIN_SIZE = 6;
static const int NODE_MIN_SIZE = ROOM_MIN_SIZE + 5; // Must be at least ROOM_MIN_SIZE + 2 to allow walls.
static const int MAX_ROOM_ITEMS = 2;
static const int MAX_ROOM_MONSTERS = 3;

namespace Game {
    class BspListener : public ITCODBspCallback { 
        private:
        Map &map; // a map to dig
        int roomNum; // room number
        int lastX, lastY; // center of the last room;
        public :
        BspListener(Map &map) : map(map), roomNum(0) {};
        bool visitNode(TCODBsp* node, void* userData);
    };
}

#ifdef _WIN32
#define UNUSED
#else
#define UNUSED __attribute__((unused))
#endif

bool Game::BspListener::visitNode(TCODBsp* node, UNUSED void* userData) {
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

Game::Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width * height];
    map = new TCODMap(width, height);
    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(NULL, 8, NODE_MIN_SIZE, NODE_MIN_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
}

Game::Map::~Map() {
    delete[] tiles;
    delete map;
}

bool Game::Map::isWall(int x, int y) const {
    return !map->isWalkable(x, y);
}

bool Game::Map::isExplored(int x, int y) const {
    return tiles[x + (y * width)].explored;
}

bool Game::Map::isInFov(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return false;
    }
    
    if (map->isInFov(x, y)) {
        tiles[x + (y * width)].explored = true;
        return true;
    }
    return false;
}

void Game::Map::computeFov() {
    map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
}

void Game::Map::setWall(int x, int y) {
    map->setProperties(x, y, true, false);    
}

void Game::Map::render() const {
    tcod::Console& console = engine.get_console();
    for(int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            TCOD_ConsoleTile& tile = console.at(x, y);
            tile.ch = ' ';
            if (isInFov(x, y)) {
                tile.bg = isWall(x, y) ? LIGHT_WALL : LIGHT_GROUND;
            } else if (isExplored(x, y)) {
                tile.bg = isWall(x, y) ? DARK_WALL : DARK_GROUND;
            }
            else
            {
                tile.bg = UNEXPLORED;
            }
        }
    }
}

void Game::Map::addItem(int x, int y) {
    TCODRandom* rng = TCODRandom::getInstance();

    int dice = rng->getInt(0, 100);
    if (dice < 70) { 
        Actor* healthPotion = new Actor(x, y, '!', "health potion", VIOLET);

        healthPotion->blocks = false;
        healthPotion->pickable = new Healer(4);
        engine.actors.push_back(healthPotion);
    } else if (dice < 70 + 10) {
        // create a scroll of lightning bolt.
        Actor* scrollOfLightningBolt = new Actor(x, y, (int)'#', "scroll of lightning bolt", LIGHT_YELLOW);
        scrollOfLightningBolt->blocks = false;
        scrollOfLightningBolt->pickable = new LightningBolt(5, 20);
        engine.actors.push_back(scrollOfLightningBolt);
    } else if (dice < 70 + 10 + 10) {
        // create a scroll of fireball
        Actor* scrollOfFireball = new Actor(x, y, (int)'#', "scroll of fireball", LIGHT_YELLOW);
        scrollOfFireball->blocks = false;
        scrollOfFireball->pickable = new Fireball(3, 12);
        engine.actors.push_back(scrollOfFireball);
    } else { //if (dice < 70 + 10 + 10 + 10) {
        // create a scroll of confusion
        Actor* scrollOfConfusion = new Actor(x, y, (int)'#', "scroll of confusion", LIGHT_YELLOW);
        scrollOfConfusion->blocks = false;
        scrollOfConfusion->pickable = new Confuser(10, 8);
        engine.actors.push_back(scrollOfConfusion);
    }
}

void Game::Map::addMonster(int x, int y) {
    TCODRandom* rng = TCODRandom::getInstance();

    if (rng->getInt(0, 100) < 80) {
        // Create an orc
        Actor* orc = new Actor(x, y, 'o', "Orc", COLOR_ORC);
        orc->destructible = new MonsterDestructible(10, 0, "dead orc");
        orc->attacker = new Attacker(3);
        orc->ai = new MonsterAi();
        engine.actors.push_back(orc);
    }
    else 
    {
        // Create a troll
        Actor* troll = new Actor(x, y, 'T', "Troll", COLOR_TROLL);
        troll->destructible = new MonsterDestructible(16, 1, "troll carcass");
        troll->attacker = new Attacker(4);
        troll->ai = new MonsterAi();
        engine.actors.push_back(troll);
    }
}

bool Game::Map::canWalk(int x, int y) const {
    if (isWall(x, y)) {
        // this is a wall
        return false;
    }

    for(auto const & actor : engine.actors) {
        if (actor->blocks && actor->x == x && actor->y == y) {
            // there is a blocking actor here. cannot walk.
            return false;
        }
    }
    return true;
}

void Game::Map::dig(int x1, int y1, int x2, int y2) {
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

void Game::Map::createRoom(bool first, int x1, int y1, int x2, int y2) {
    dig(x1, y1, x2, y2);
    if (first) {
        engine.player->x = (x1 + x2) / 2;
        engine.player->y = (y1 + y2) / 2;        
    } else {
        TCODRandom* rng = TCODRandom::getInstance();
        int countMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
        while (countMonsters > 0) {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if (canWalk(x, y)) {
                addMonster(x, y);
            }
            countMonsters--;
        }

        int countItems = rng->getInt(0, MAX_ROOM_ITEMS);
        while (countItems > 0) {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if (canWalk(x, y)) {
                addItem(x, y);
            }
            countItems--;
        }
    }
}