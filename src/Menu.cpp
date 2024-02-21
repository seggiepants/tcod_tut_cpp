#include "Menu.hpp"
#include "Engine.hpp"

Game::Menu::Menu() : selectedItem(0), mouseX(0), mouseY(0) {}

Game::Menu::~Menu() { destroy(); }

void Game::Menu::clear() {
  for (auto& menuItem : items) {
    delete menuItem;
  }
  items.clear();
}

void Game::Menu::addItem(MenuItemCode code, const char* label) {
  MenuItem* item = new MenuItem();
  item->code = code;
  item->label.assign(label);
  items.push_back(item);
}

void Game::Menu::init() {
  bool hasSaveFile = false;

#ifndef __EMSCRIPTEN__
  std::filesystem::path path(SAVE_FILENAME);
  if (std::filesystem::exists(path)) {
    hasSaveFile = true;
  }
#endif
  clear();
  addItem(Menu::NEW_GAME, "New Game");
  if (hasSaveFile) {
    addItem(Menu::CONTINUE, "Continue");
  }
  addItem(Menu::EXIT, "Exit");
}

void Game::Menu::destroy() {
  clear();
}

Game::Scene* Game::Menu::processMenu(MenuItemCode selection) {
  // enum MenuItemCode { NONE, NEW_GAME, CONTINUE, EXIT };
  if (selection == Menu::EXIT || selection == Menu::NONE) {
    engine.Stop();
    return engine.scenes[GameScene::EXIT];
  } else if (selection == Menu::NEW_GAME) {
    Game* game = (Game*)engine.scenes[GameScene::GAME];
    game->init();
    return game;
  } else if (selection == Menu::CONTINUE) {
    Game* game = (Game*)engine.scenes[GameScene::GAME];
    game->load();
    return game;
  }
  return (Scene*)this;
}

Game::Scene* Game::Menu::update() {
  int currentKey, dy = 0;
  Scene* next = this;
  // Wait for a key up or exit
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    dy = 0;
    switch (event.type) {
      case SDL_QUIT:
        engine.Stop();
        return next = processMenu(MenuItemCode::EXIT);
        break;
      case SDL_KEYDOWN: {
        currentKey = event.key.keysym.sym;
        switch (currentKey) {
          case SDLK_ESCAPE:
            return processMenu(MenuItemCode::EXIT);
            break;
          case SDLK_RETURN:
          case SDLK_RETURN2:
          case SDLK_KP_ENTER: {
            std::list<MenuItem*>::iterator iter = items.begin();
            std::advance(iter, selectedItem);
            return processMenu((MenuItemCode)((*iter)->code));
          } break;
          case SDLK_UP:
          case SDLK_KP_8:
            dy = -1;
            break;
          case SDLK_DOWN:
          case SDLK_KP_2:
            dy = 1;
            break;
        }
        if (dy != 0) {
          selectedItem += dy;
          if (selectedItem < 0) selectedItem = (int)items.size() - 1;
          if (selectedItem >= (int)items.size()) selectedItem = 0;
        }
      } break;
      case SDL_MOUSEBUTTONDOWN: {
        switch (event.button.button) {
          case SDL_BUTTON_LEFT: {
            // Select and return on click.
            int index = 0;
            printf("clicked on %d, %d", mouseX, mouseY);
            for (auto& item : items) {
              if (mouseX >= 10 && mouseX <= 10 + (int)item->label.size() && mouseY == 10 + (3 * index)) {
                return processMenu(item->code);
                break;
              }
              index++;
            }
          } break;
        }
      } break;
      case SDL_MOUSEMOTION:
        // change selection on mouse move.
        {
          engine.get_context().convert_event_coordinates(event);
          mouseX = event.motion.x;
          mouseY = event.motion.y;
          int index = 0;
          for (auto& item : items) {
            if (mouseX >= 10 && mouseX <= 10 + (int)item->label.size() && mouseY == 10 + (3 * index)) {
              selectedItem = index;
              break;
            }
            index++;
          }
        }
        break;
    }
  }
  return next;
}

void Game::Menu::render() {
  static std::filesystem::path imagePath = engine.GetRootDir() / "data" / "menu_background1.png";
  static std::string imgPath(imagePath.string());
  static TCODImage img(imgPath.c_str());

  tcod::draw_quartergraphics(engine.get_console(), img, {0, 0}, {0, 0, -1, -1});
  int currentItem = 0;
  for (auto& item : items) {
    tcod::print(
        engine.get_console(),
        {10, 10 + (currentItem * 3)},
        item->label.c_str(),
        currentItem == selectedItem ? black : lightGrey,
        currentItem == selectedItem ? lighterOrange : black,
        TCOD_LEFT,
        TCOD_BKGND_SET);
    currentItem++;
  }
  // ZZZ engine.flush();
}

