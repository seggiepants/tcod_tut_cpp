#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <SDL.h>

#include <filesystem>
#include <libtcod.hpp>
#include <list>
#include <unordered_map>

#include "Actor.hpp"
#include "Game.hpp"
#include "Gui.hpp"
#include "Map.hpp"
#include "Menu.hpp"
#include "Pickable.hpp"
#include "Scene.hpp"

namespace Game {
enum GameScene { EXIT, MENU, GAME, PICKTILE, PICKINVENTORY };

enum InventoryCommand { NONE, USE, DROP };

class Engine {
 public:
  Engine();
  ~Engine();
  void init(int argc, char** argv, int screenWidth, int screenHeight);
  void destroy();
  void update();
  void render();
  void present() { context.present(console); };
  Actor* getActor(int x, int y) const;
  bool isRunning() { return running; }
  void pickATile(Pickable* source, Actor* owner, Actor* wearer, float maxRange = 0.0f);
  bool tilePicked(bool success, int x, int y);
  void pickInventory(Actor* owner, InventoryCommand cmd);
  bool inventoryPicked(bool success, Actor* owner, Actor* wearer);
  void Stop() { running = false; }
  void sendToBack(Actor* actor);
  tcod::Console& get_console() { return console; };
  tcod::Context& get_context() { return context; };
  Actor* getClosestMonster(int x, int y, float range = 0.0f) const;
  Gui* get_gui();

  enum GameStatus { STARTUP, IDLE, NEW_TURN, VICTORY, DEFEAT } gameStatus;
  int get_width() { return screenWidth; }
  int get_height() { return screenHeight; }
  bool mouseClicked;
  int mouseX;  // mouse position in tiles/characters
  int mouseY;
  std::filesystem::path GetRootDir();
  std::unordered_map<GameScene, Scene*> scenes;
  Scene* currentScene;

 protected:
  tcod::Console console;
  tcod::Context context;
  bool running;
  bool initialized;
  int screenWidth;
  int screenHeight;
  Pickable* tilePicker;
  Actor* pickOwner;
  Actor* pickWearer;
  InventoryCommand inventoryCommand;
};
}  // namespace Game

extern TCOD_ColorRGBA BLACK;
extern TCOD_ColorRGBA BLUE;
extern TCOD_ColorRGBA CYAN;
extern TCOD_ColorRGBA DARKER_VIOLET;
extern TCOD_ColorRGBA GREEN;
extern TCOD_ColorRGBA LIGHT_BLUE;
extern TCOD_ColorRGBA LIGHT_GREEN;
extern TCOD_ColorRGBA LIGHT_YELLOW;
extern TCOD_ColorRGBA LIGHTER_ORANGE;
extern TCOD_ColorRGBA LIGHT_VIOLET;
extern TCOD_ColorRGBA MAGENTA;
extern TCOD_ColorRGBA ORANGE;
extern TCOD_ColorRGBA RED;
extern TCOD_ColorRGBA WHITE;
extern TCOD_ColorRGBA VIOLET;
extern TCOD_ColorRGBA YELLOW;
extern TCOD_ColorRGBA COLOR_CORPSE;
extern TCOD_ColorRGBA COLOR_ORC;
extern TCOD_ColorRGBA COLOR_TROLL;
extern TCOD_ColorRGBA DARK_GROUND;
extern TCOD_ColorRGBA DARK_WALL;
extern TCOD_ColorRGBA LIGHT_GROUND;
extern TCOD_ColorRGBA LIGHT_WALL;
extern TCOD_ColorRGBA UNEXPLORED;

extern TCOD_ColorRGB black;
extern TCOD_ColorRGB cyan;
extern TCOD_ColorRGB darkerRed;
extern TCOD_ColorRGB darkerViolet;
extern TCOD_ColorRGB lightBlue;
extern TCOD_ColorRGB lightGreen;
extern TCOD_ColorRGB lightGrey;
extern TCOD_ColorRGB lightRed;
extern TCOD_ColorRGB lightYellow;
extern TCOD_ColorRGB lighterOrange;
extern TCOD_ColorRGB lightViolet;
extern TCOD_ColorRGB orange;
extern TCOD_ColorRGB red;
extern TCOD_ColorRGB white;
extern TCOD_ColorRGB yellow;

extern Game::Engine engine;

extern const char* SAVE_FILENAME;

#endif
