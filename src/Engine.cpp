#include "Engine.hpp"

#include <SDL.h>

#include <fstream>
#include <iostream>

#include "Attacker.hpp"
#include "Container.hpp"
#include "PickInventory.hpp";
#include "PickTile.hpp"
#include "PlayerAi.hpp"
#include "PlayerDestructible.hpp"

TCOD_ColorRGBA BLACK = {0, 0, 0, 255};
TCOD_ColorRGBA BLUE = {0, 0, 255, 255};
TCOD_ColorRGBA CYAN = {0, 255, 255, 255};
TCOD_ColorRGBA DARKER_VIOLET = {63, 0, 127, 255};
TCOD_ColorRGBA GREEN = {0, 255, 0, 255};
TCOD_ColorRGBA LIGHT_BLUE = {63, 63, 255, 255};
TCOD_ColorRGBA LIGHT_GREEN = {63, 255, 63, 255};
TCOD_ColorRGBA LIGHT_YELLOW = {255, 255, 63, 255};
TCOD_ColorRGBA LIGHTER_ORANGE = {255, 191, 127, 255};
TCOD_ColorRGBA LIGHT_VIOLET = {159, 63, 255, 255};
TCOD_ColorRGBA MAGENTA = {255, 0, 255, 255};
TCOD_ColorRGBA ORANGE = {255, 127, 0, 255};
TCOD_ColorRGBA RED = {255, 0, 0, 255};
TCOD_ColorRGBA WHITE = {255, 255, 255, 255};
TCOD_ColorRGBA VIOLET = {127, 0, 255, 255};
TCOD_ColorRGBA YELLOW = {255, 255, 0, 255};
TCOD_ColorRGBA COLOR_CORPSE = {191, 0, 0, 255};
TCOD_ColorRGBA COLOR_ORC = {63, 127, 63, 255};
TCOD_ColorRGBA COLOR_TROLL = {0, 127, 0, 255};
TCOD_ColorRGBA DARK_GROUND = {50, 50, 150, 255};
TCOD_ColorRGBA DARK_WALL = {0, 0, 100, 255};
TCOD_ColorRGBA LIGHT_GROUND = {200, 180, 50, 255};
TCOD_ColorRGBA LIGHT_WALL = {130, 110, 50, 255};
TCOD_ColorRGBA UNEXPLORED = {0, 0, 0, 255};

TCOD_ColorRGB black = {0, 0, 0};
TCOD_ColorRGB cyan = {0, 255, 255};
TCOD_ColorRGB darkerRed = {127, 0, 0};
TCOD_ColorRGB darkerViolet = {63, 0, 127};
TCOD_ColorRGB lightBlue = {63, 63, 255};
TCOD_ColorRGB lightGreen = {63, 255, 63};
TCOD_ColorRGB lightGrey = {159, 159, 159};
TCOD_ColorRGB lightRed = {255, 63, 63};
TCOD_ColorRGB lightYellow = {255, 255, 63};
TCOD_ColorRGB lighterOrange = {255, 191, 127};
TCOD_ColorRGB lightViolet = {159, 63, 255};
TCOD_ColorRGB orange = {255, 127, 0};
TCOD_ColorRGB red = {255, 0, 0};
TCOD_ColorRGB white = {255, 255, 255};
TCOD_ColorRGB yellow = {255, 255, 0};

const char* SAVE_FILENAME = "game.sav";

Game::Engine::Engine() { initialized = false; }

Game::Engine::~Engine() { destroy(); }

void Game::Engine::destroy() {
  for (auto& value : scenes) {
    if (value.second != nullptr) {
      value.second->destroy();
      delete value.second;
    }
  }
  scenes.clear();
}

void Game::Engine::init(int argc, char** argv, int screenWidth, int screenHeight) {
  if (initialized) return;

  this->screenWidth = screenWidth;
  this->screenHeight = screenHeight;

  try {
    auto params = TCOD_ContextParams{};
    params.tcod_version = TCOD_COMPILEDVERSION;
    params.argc = argc;
    params.argv = argv;
    params.renderer_type = TCOD_RENDERER_SDL2;
    params.vsync = 1;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.window_title = "Libtcod C++ Tutorial";
    auto tileset = tcod::load_tilesheet(GetRootDir() / "data" / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();

    console = tcod::Console{screenWidth, screenHeight};
    SDL_RenderSetVSync(context.get_sdl_renderer(), 1);  // turn on vsync (might help emscripten).

    params.console = console.get();

    context = tcod::Context(params);
    gameStatus = STARTUP;
    running = false;
    initialized = true;

    destroy();
    scenes[GameScene::EXIT] = nullptr;
    scenes[GameScene::GAME] = new Game();
    scenes[GameScene::MENU] = new Menu();
    scenes[GameScene::PICKTILE] = new PickTile();
    scenes[GameScene::PICKINVENTORY] = new PickInventory();
    currentScene = scenes[GameScene::MENU];
    ((Menu*)currentScene)->mode = Menu::MENU;    
    currentScene->init();
    running = true;

  } catch (const std::exception& exc) {
    std::cerr << exc.what() << "\n";
    throw;
  }
}

void Game::Engine::update() {
  if (!currentScene) return;

  Scene* next = currentScene->update();
  currentScene = next;
}

void Game::Engine::render() {
  if (!currentScene) return;

  currentScene->render();
}

void Game::Engine::pickATile(Pickable* source, Actor* owner, Actor* wearer, float maxRange) {
  PickTile* scene = (PickTile*)scenes[GameScene::PICKTILE];
  scene->init();
  scene->maxRange = maxRange;
  tilePicker = source;
  pickOwner = owner;
  pickWearer = wearer;
  currentScene = scene;
}
bool Game::Engine::tilePicked(bool success, int x, int y) {
  currentScene = scenes[GameScene::GAME];
  if (success) {
    tilePicker->picked(pickOwner, pickWearer, success, x, y);
  }
  return success;
}

void Game::Engine::pickInventory(Actor* owner, InventoryCommand cmd) {
  PickInventory* scene = (PickInventory*)scenes[GameScene::PICKINVENTORY];
  scene->init();
  scene->owner = owner;
  inventoryCommand = cmd;
  currentScene = scene;
}

bool Game::Engine::inventoryPicked(bool success, Actor* owner, Actor* wearer) {
  Game* game = (Game*)scenes[GameScene::GAME];
  currentScene = game;
  if (success) {
    switch (inventoryCommand) {
      case InventoryCommand::USE:
        ((PlayerAi*)game->player->ai)->useInventory(owner, wearer);
        break;
      case InventoryCommand::DROP:
        ((PlayerAi*)game->player->ai)->dropInventory(owner, wearer);
        break;
    }
  }
  return success;
}

Game::Gui* Game::Engine::get_gui() { return ((Game*)scenes[GameScene::GAME])->gui; }

void Game::Engine::sendToBack(Actor* actor) {
  Game* game = ((Game*)scenes[GameScene::GAME]);
  game->actors.remove(actor);
  game->actors.push_front(actor);
}

Game::Actor* Game::Engine::getClosestMonster(int x, int y, float range) const {
  Actor* closest = nullptr;
  Game* game = (Game*)scenes.at(GameScene::GAME);

  float bestDistance = 1E6f;  // start with a really big number.

  for (auto const& actor : game->actors) {
    if (actor != game->player && actor->destructible && !actor->destructible->isDead()) {
      float distance = actor->getDistance(x, y);
      if (distance < bestDistance && (distance <= range || range == 0.0f)) {
        bestDistance = distance;
        closest = actor;
      }
    }
  }

  return closest;
}

Game::Actor* Game::Engine::getActor(int x, int y) const {
  Game* game = (Game*)scenes.at(GameScene::GAME);
  for (auto const& actor : game->actors) {
    if (actor->x == x && actor->y == y && actor->destructible && !actor->destructible->isDead()) {
      return actor;
    }
  }
  return nullptr;
}

/// Return the data directory.
std::filesystem::path Game::Engine::GetRootDir() {
  static auto root_directory = std::filesystem::path{"."};  // Begin at the working directory.
  while (!std::filesystem::exists(root_directory / "data")) {
    // If the current working directory is missing the data dir then it will assume it exists in any parent directory.
    root_directory /= "..";
    if (!std::filesystem::exists(root_directory)) {
      throw std::runtime_error("Could not find the data directory.");
    }
  }
  return root_directory;
};
