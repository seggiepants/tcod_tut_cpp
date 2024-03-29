#include "Game.hpp"

#include <SDL.h>

#include <iostream>

#include "Container.hpp"
#include "Engine.hpp"
#include "PlayerAi.hpp"
#include "PlayerDestructible.hpp"

Game::Game::Game() : gameStatus(STARTUP), currentKey((SDL_Keycode)0), fovRadius(10), level(1) {
  player = nullptr;
  map = nullptr;
  gui = nullptr;
  initialized = false;
}

Game::Game::~Game() {
  destroy();
  delete gui;
  gui = nullptr;
}

void Game::Game::init() {
  destroy();  // Make sure everything is clean

  player = new Actor(engine.get_console().get_width() / 2, engine.get_console().get_height() / 2, '@', "player", WHITE);
  player->destructible = new PlayerDestructible(30, 2, "your cadaver", 0);
  player->attacker = new Attacker(5);
  player->ai = new PlayerAi();
  player->container = new Container(26);

  stairs = new Actor(0, 0, '>', "stairs", WHITE);
  stairs->blocks = false;
  stairs->fovOnly = false;

  if (!gui) gui = new Gui();

  map = new Map(engine.get_console().get_width(), engine.get_console().get_height() - gui->get_height());
  map->init(true);
  actors.push_back(stairs); // Make sure stairs 2nd to top
  actors.push_back(player); // with Player on top for Z-Ordering.
  gui->message(red, "Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.");
  gameStatus = GameStatus::STARTUP;
}

void Game::Game::destroy() {
  for (auto& actor : actors) {
    delete actor;
  }
  actors.clear();
  player = nullptr;

  if (map) {
    delete map;
    map = nullptr;
  }

  if (gui) {
    gui->clear();
  }
}

Game::Scene* Game::Game::update() {
  // Handle input.
  SDL_Event event;
  currentKey = (SDL_Keycode)0;  // Clear each time  only new keys stay
  mouseClicked = false;  // Clear each time only want current status for the frame.

  if (gameStatus == STARTUP) {
    // Compute FOV on first frame.
    map->computeFov();
  }
  gameStatus = IDLE;

#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        // std::exit(EXIT_SUCCESS);
        engine.Stop();
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
          {
            save();
            Scene* next = engine.scenes[GameScene::MENU];
            ((Menu*)next)->mode = Menu::MENU;
            next->init();
            engine.currentScene = next;
            // engine.load();
          }
          break;
          default:
            currentKey = event.key.keysym.sym;
            break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button) {
          case SDL_BUTTON_LEFT:
          case SDL_BUTTON_RIGHT:
            // Split this out later - left move, right look

            break;
        }
        break;
      case SDL_MOUSEMOTION:
        engine.get_context().convert_event_coordinates(event);
        mouseX = event.motion.x;
        mouseY = event.motion.y;
    }
  }

  player->update();
  if (gameStatus == NEW_TURN) {
    for (auto const& actor : actors) {
      if (actor != player) {
        actor->update();
      }
    }
  }
  return engine.currentScene;
}

void Game::Game::render() {
  engine.get_console().clear();
  map->render();
  for (auto const& actor : actors) {
    if ((!actor->fovOnly && map->isExplored(actor->x, actor->y)) || (map->isInFov(actor->x, actor->y))) {
      actor->render();
    }
  }
  gui->render();
}

void Game::Game::nextLevel() {
  int mapWidth, mapHeight;
  level++;
  gui->message(lightGrey, "You take a moment to rest, and recover your strength.");
  player->destructible->heal(player->destructible->maxHp / 2);
  gui->message(red, "After a rare moment of peace, you decend\ndeeper into the heart of the dungeon...");

  mapWidth = map->width;
  mapHeight = map->height;
  delete map;
  for (auto it = actors.begin(); it != actors.end(); it++) { // Will be doing deletion iterator sounds better than auto for loop for that.
    if (*it != player && *it != stairs) {
      delete *it;
    }
  }
  actors.clear(); // I will just re-add stairs and player at the end of the repopulated actor list
  map = new Map(mapWidth, mapHeight);
  map->init(true);
  actors.push_back(stairs);
  actors.push_back(player);
  gameStatus = GameStatus::STARTUP;
}

void Game::Game::load() {
  bool hasSaveFile = false;

#ifdef __EMSCRIPTEN__
  hasSaveFile = !player->destructible->isDead() && (gameStatus != GameStatus::STARTUP || player->destructible->xp > 0);
  gameStatus = GameStatus::STARTUP;
  return;
#else
  std::filesystem::path path(SAVE_FILENAME);
  if (std::filesystem::exists(path)) {
    hasSaveFile = true;
  }
#endif

  if (!hasSaveFile) {
    destroy();
    init();
  } else {  // selection == MenuItemCode::CONTINUE
    std::ifstream stream(SAVE_FILENAME, std::ios::binary | std::ios::in);
    // cereal::JSONInputArchive archive(fs);

    destroy();

    // load the map
    int width, height;
    // archive(width, height);
    char delim = ',';
    stream >> width >> delim >> height >> delim;
    map = new Map(width, height);
    map->width = width;
    map->height = height;
    map->init(false);
    gui = new Gui();
    map = new Map(engine.get_console().get_width(), engine.get_console().get_height() - gui->get_height());
    map->load(stream);

    int actorCount = 0;
    stream >> actorCount >> delim;
    for (int i = 0; i < actorCount; ++i) {
      Actor* currentActor = new Actor();
      currentActor->load(stream);
      stream >> delim >> delim;
      actors.push_back(currentActor);
    }

    // reference the player
    for (auto const& actor : actors) {
      if (actor->ch == (int)'@') {
        player = actor;
      } else if (actor->ch == (int)'>') {
        stairs = actor;
      }
    }

    gui->load(stream);

    stream.close();
    gameStatus = GameStatus::STARTUP;
  }
}

void Game::Game::save() {
#ifdef __EMSCRIPTEN__
  return;
#else
  const char delim = ',';
  if (!player) return;
  if (!player->destructible) return;

  if (player->destructible->isDead()) {
    std::filesystem::path path(SAVE_FILENAME);
    if (std::filesystem::exists(path)) {
      std::filesystem::remove(path);
    }
  } else {
    std::ofstream stream(SAVE_FILENAME, std::ios::binary | std::ios::out);

    // save the map first
    stream << map->width << delim << map->height << delim;
    map->save(stream);

    // archive(actors);

    stream << actors.size() << delim;
    for (auto const& actor : actors) {
      actor->save(stream);
      stream << '~' << delim;
    }

    gui->save(stream);

    stream.flush();
    stream.close();
  }
#endif
}
