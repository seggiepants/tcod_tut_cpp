#include <iostream>
#include <SDL.h>
#include "Game.hpp"
#include "Engine.hpp"
#include "PlayerAi.hpp"
#include "PlayerDestructible.hpp"
#include "Container.hpp"

Game::Game::Game() : gameStatus(STARTUP), fovRadius(10), currentKey((SDL_Keycode)0) {
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
  player->destructible = new PlayerDestructible(30, 2, "your cadaver");
  player->attacker = new Attacker(5);
  player->ai = new PlayerAi();
  player->container = new Container(26);
  if (!gui) gui = new Gui();

  map = new Map(engine.get_console().get_width(), engine.get_console().get_height() - gui->get_height());
  map->init(true);
  actors.push_back(player);  // Make sure player is on-top.
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
#ifndef __EMSCRIPTEN__
          {
            save();
            Scene* next = engine.scenes[GameScene::MENU];
            next->init();
            engine.currentScene = next;
            // engine.load();
          }
#endif
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
    if (map->isInFov(actor->x, actor->y)) {
      actor->render();
    }
  }
  gui->render();
}

void Game::Game::load() {
  bool hasSaveFile = false;

#ifndef __EMSCRIPTEN__
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
        break;
      }
    }

    gui->load(stream);

    stream.close();
    gameStatus = GameStatus::STARTUP;
  }
}

void Game::Game::save() {
#ifndef __EMSCRIPTEN__
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
