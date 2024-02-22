#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <list>

#include "Actor.hpp"
#include "Gui.hpp"
#include "Map.hpp"
#include "Scene.hpp"

namespace Game {

class Game : public Scene {
 public:
  Game();
  ~Game();
  void render();
  Scene* update();
  void init();
  void destroy();
  void load();
  void save();
  void useInventory(Actor* owner, Actor* item);
  void dropInventory(Actor* owner, Actor* item);
  void nextLevel();
  int get_level() { return level; };

  enum GameStatus { STARTUP, IDLE, NEW_TURN, VICTORY, DEFEAT } gameStatus;

  std::list<Actor*> actors;
  Actor* player;
  Actor* stairs;
  Map* map;
  Gui* gui;
  int currentKey;
  int fovRadius;

 protected:
  bool mouseClicked;
  int mouseX;  // mouse position in tiles/characters
  int mouseY;
  int level;
  bool initialized;
};
}  // namespace Game
#endif
