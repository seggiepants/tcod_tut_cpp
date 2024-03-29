#include "Map.hpp"

#include "Attacker.hpp"
#include "Confuser.hpp"
#include "Engine.hpp"
#include "Fireball.hpp"
#include "Healer.hpp"
#include "LightningBolt.hpp"
#include "MonsterAi.hpp"
#include "MonsterDestructible.hpp"

// Max size not used yet.
static const int ROOM_MIN_SIZE = 6;
static const int NODE_MIN_SIZE = ROOM_MIN_SIZE + 5;  // Must be at least ROOM_MIN_SIZE + 2 to allow walls.
static const int MAX_ROOM_ITEMS = 2;
static const int MAX_ROOM_MONSTERS = 3;

namespace Game {
class BspListener : public ITCODBspCallback {
 private:
  Map& map;  // a map to dig
  int roomNum;  // room number
  int lastX, lastY;  // center of the last room;
 public:
  BspListener(Map& map) : map(map), roomNum(0){};
  bool visitNode(TCODBsp* node, void* userData);
};
}  // namespace Game

#ifdef _WIN32
#define UNUSED
#else
#define UNUSED __attribute__((unused))
#endif

bool Game::BspListener::visitNode(TCODBsp* node, UNUSED void*) {
  if (node->isLeaf()) {
    int x, y, w, h;

    // dig a room
    w = map.rng->getInt(ROOM_MIN_SIZE, node->w - 2);
    h = map.rng->getInt(ROOM_MIN_SIZE, node->h - 2);
    x = map.rng->getInt(node->x + 1, node->x + node->w - w - 1);
    y = map.rng->getInt(node->y + 1, node->y + node->h - h - 1);
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
  tiles = nullptr;
  map = nullptr;
}

void Game::Map::init(bool generateRooms) {
  rng = new TCODRandom();

  if (tiles) delete[] tiles;

  tiles = new Tile[width * height];

  if (map) delete map;
  map = new TCODMap(width, height);

  if (generateRooms) {
    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(rng, 8, NODE_MIN_SIZE, NODE_MIN_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, nullptr);
  }
}

Game::Map::~Map() {
  delete[] tiles;
  delete map;
}

bool Game::Map::isWall(int x, int y) const { return !map->isWalkable(x, y); }

bool Game::Map::isExplored(int x, int y) const { return tiles[x + (y * width)].explored; }

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
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  map->computeFov(game->player->x, game->player->y, game->fovRadius);
}

void Game::Map::setWall(int x, int y) { map->setProperties(x, y, true, false); }

void Game::Map::render() const {
  tcod::Console& console = engine.get_console();
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      TCOD_ConsoleTile& tile = console.at(x, y);
      tile.ch = ' ';
      if (isInFov(x, y)) {
        tile.bg = isWall(x, y) ? LIGHT_WALL : LIGHT_GROUND;
      } else if (isExplored(x, y)) {
        tile.bg = isWall(x, y) ? DARK_WALL : DARK_GROUND;
      } else {
        tile.bg = UNEXPLORED;
      }
    }
  }
}

void Game::Map::addItem(int x, int y) {
  rng = TCODRandom::getInstance();
  Game* game = (Game*)engine.scenes[GameScene::GAME];

  int dice = rng->getInt(0, 100);
  if (dice < 70) {
    Actor* healthPotion = new Actor(x, y, '!', "health potion", VIOLET);
    healthPotion->blocks = false;
    healthPotion->pickable = new Healer(4);
    game->actors.push_back(healthPotion);
  } else if (dice < 70 + 10) {
    // create a scroll of lightning bolt.
    Actor* scrollOfLightningBolt = new Actor(x, y, (int)'#', "scroll of lightning bolt", LIGHT_YELLOW);
    scrollOfLightningBolt->blocks = false;
    scrollOfLightningBolt->pickable = new LightningBolt(6, 20);
    game->actors.push_back(scrollOfLightningBolt);
  } else if (dice < 70 + 10 + 10) {
    // create a scroll of fireball
    Actor* scrollOfFireball = new Actor(x, y, (int)'#', "scroll of fireball", LIGHT_YELLOW);
    scrollOfFireball->blocks = false;
    scrollOfFireball->pickable = new Fireball(4, 12);
    game->actors.push_back(scrollOfFireball);
  } else {  // if (dice < 70 + 10 + 10 + 10) {
    // create a scroll of confusion
    Actor* scrollOfConfusion = new Actor(x, y, (int)'#', "scroll of confusion", LIGHT_YELLOW);
    scrollOfConfusion->blocks = false;
    scrollOfConfusion->pickable = new Confuser(10, 8);
    game->actors.push_back(scrollOfConfusion);
  }
}

void Game::Map::addMonster(int x, int y) {
  TCODRandom* rng = TCODRandom::getInstance();
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  if (rng->getInt(0, 100) < 80) {
    // Create an orc
    Actor* orc = new Actor(x, y, 'o', "Orc", COLOR_ORC);
    orc->destructible = new MonsterDestructible(10, 0, "dead orc", 10);
    orc->attacker = new Attacker(3);
    orc->ai = new MonsterAi();
    game->actors.push_back(orc);
  } else {
    // Create a troll
    Actor* troll = new Actor(x, y, 'T', "Troll", COLOR_TROLL);
    troll->destructible = new MonsterDestructible(16, 1, "troll carcass", 25);
    troll->attacker = new Attacker(4);
    troll->ai = new MonsterAi();
    game->actors.push_back(troll);
  }
}

bool Game::Map::canWalk(int x, int y) const {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  if (isWall(x, y)) {
    // this is a wall
    return false;
  }

  for (auto const& actor : game->actors) {
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
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  dig(x1, y1, x2, y2);

  if (first) {
    game->player->x = (x1 + x2) / 2;
    game->player->y = (y1 + y2) / 2;
  } else {
    // TCODRandom* rng = TCODRandom::getInstance();
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

  game->stairs->x = (x1 + x2) / 2;
  game->stairs->y = (y1 + y2) / 2; // Update as you go, last one sticks.
}

void Game::Map::load(std::ifstream& stream) {
  char delim = ',';
  init(false);
  char* buffer = new char[width * height];
  stream.read(buffer, width * height);
  int rowIdx;
  for (int j = 0; j < height; j++) {
    rowIdx = width * j;
    for (int i = 0; i < width; i++) {
      bool bWalkable = buffer[rowIdx + i] == '1' ? true : false;
      map->setProperties(i, j, bWalkable, bWalkable);
    }
  }
  stream >> delim;
  stream.read(buffer, width * height);
  for (int i = 0; i < width * height; i++) {
    tiles[i].explored = (buffer[i] == '1');
  }
  stream >> delim;
  delete[] buffer;
}

void Game::Map::save(std::ofstream& stream) {
  const char delim = ',';
  char* buffer = new char[width * height];
  int rowIdx;
  for (int j = 0; j < height; j++) {
    rowIdx = width * j;
    for (int i = 0; i < width; i++) {
      buffer[rowIdx + i] = map->isWalkable(i, j) ? '1' : '0';
    }
  }
  stream.write(buffer, width * height);
  stream << delim;

  for (int i = 0; i < width * height; i++) {
    buffer[i] = tiles[i].explored ? '1' : '0';
  }
  stream.write(buffer, width * height);
  stream << delim;
  delete[] buffer;
}
