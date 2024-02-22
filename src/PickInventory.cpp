#include "PickInventory.hpp"

#include <libtcod.hpp>

#include "Container.hpp"
#include "Engine.hpp"
#include "Game.hpp"

Game::PickInventory::PickInventory() : owner(nullptr) {}

void Game::PickInventory::init() {}

void Game::PickInventory::destroy() { owner = nullptr; }

Game::Scene* Game::PickInventory::update() {
  Game* game = (Game*)engine.scenes[GameScene::GAME];

  SDL_Event event;
  SDL_Keycode currentKey;

  // Wait for a key up or exit
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        engine.Stop();
        return nullptr;
        break;
      case SDL_KEYDOWN:
        currentKey = event.key.keysym.sym;
        if (currentKey >= SDLK_a && currentKey <= SDLK_z) {
          int index = currentKey - SDLK_a;
          if (index >= 0 && (size_t)index < owner->container->inventory.size()) {
            std::list<Actor*>::iterator iter = owner->container->inventory.begin();
            std::advance(iter, index);
            engine.inventoryPicked(true, (*iter), owner);
          } else {
            engine.inventoryPicked(false, nullptr, owner);
          }
        } else if (currentKey == SDLK_ESCAPE) {
          engine.inventoryPicked(false, nullptr, owner);
        }

        break;
    }
  }
  return engine.currentScene;
}

void Game::PickInventory::render() {
  static const int INVENTORY_WIDTH = 50;
  static const int INVENTORY_HEIGHT = 28;  // 26 letters in alphabet + 2 for border.

  Game* game = (Game*)engine.scenes[GameScene::GAME];
  game->render();

  tcod::Console con(INVENTORY_WIDTH, INVENTORY_HEIGHT);

  // display the inventory frame
  TCOD_ConsoleTile tile;
  tile.bg = LIGHT_GROUND;
  tile.fg = WHITE;
  tile.ch = ' ';
  con.clear(tile);
  TCOD_console_printf_frame(con.get(), 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "inventory");
  int shortcut = (int)'a';
  int y = 1;
  for (auto const& actor : owner->container->inventory) {
    TCOD_console_printf(con.get(), 2, y, "(%c) %s", shortcut, actor->name.c_str());
    y++;
    shortcut++;
  }
  tcod::blit(
      engine.get_console(),
      con,
      {(engine.get_console().get_width() - INVENTORY_WIDTH) / 2,
       (engine.get_console().get_height() - INVENTORY_HEIGHT) / 2},
      {0, 0, con.get_width(), con.get_height()},
      1.0f,
      1.0f);
  // ZZZ engine.flush();
}
