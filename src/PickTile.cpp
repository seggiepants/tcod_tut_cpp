#include "PickTile.hpp"

#include <SDL.h>

#include "Engine.hpp"
#include "Game.hpp"

Game::PickTile::PickTile() : maxRange(0), x(0), y(0) {}

void Game::PickTile::init() {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  mouseX = game->player->x;  // This might fail on chromebook.
  mouseY = game->player->y;  // going to add cursor keys as a fall-back.
}

void Game::PickTile::destroy() {}

Game::Scene* Game::PickTile::update() {
  Game* game = (Game*)engine.scenes[GameScene::GAME];

  SDL_Event event;
  int dx = 0;
  int dy = 0;

  // Wait for a key up or exit
  while (SDL_PollEvent(&event)) {
    dx = 0;
    dy = 0;
    switch (event.type) {
      case SDL_QUIT:
        engine.Stop();
        return engine.scenes[GameScene::EXIT];
        break;
      case SDL_KEYDOWN: {
        currentKey = event.key.keysym.sym;
        switch (currentKey) {
          case SDLK_ESCAPE:
            engine.tilePicked(false, mouseX, mouseY);
            break;
          case SDLK_RETURN:
          case SDLK_RETURN2:
          case SDLK_KP_ENTER:
            engine.tilePicked(isPositionOK(mouseX, mouseY), mouseX, mouseY);
            break;
          case SDLK_KP_9:
            dy = -1;
            dx = 1;
            break;
          case SDLK_UP:
          case SDLK_KP_8:
            dy = -1;
            break;
          case SDLK_KP_7:
            dy = -1;
            dx = -1;
            break;
          case SDLK_RIGHT:
          case SDLK_KP_6:
            dx = 1;
            break;
          case SDLK_LEFT:
          case SDLK_KP_4:
            dx = -1;
            break;
          case SDLK_KP_3:
            dy = 1;
            dx = 1;
            break;
          case SDLK_DOWN:
          case SDLK_KP_2:
            dy = 1;
            break;
          case SDLK_KP_1:
            dy = 1;
            dx = -1;
            break;
        }

        if (dx != 0 || dy != 0) {
          mouseX += dx;
          mouseY += dy;
        }
      } break;
      case SDL_MOUSEBUTTONDOWN: {
        switch (event.button.button) {
          case SDL_BUTTON_LEFT:
            engine.tilePicked(isPositionOK(mouseX, mouseY), mouseX, mouseY);
            break;
          case SDL_BUTTON_RIGHT:
            engine.tilePicked(false, mouseX, mouseY);
            break;
        }
      } break;
      case SDL_MOUSEMOTION:
        engine.get_context().convert_event_coordinates(event);
        mouseX = event.motion.x;
        mouseY = event.motion.y;
        break;
    }
  }
  // flush();
  return engine.currentScene;
}

void Game::PickTile::render() {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  game->render();
  // highlight the possible range
  for (int cy = 0; cy < game->map->height; ++cy) {
    for (int cx = 0; cx < game->map->width; ++cx) {
      if (game->map->isInFov(cx, cy) && (maxRange == 0.0f || game->player->getDistance(cx, cy) < maxRange)) {
        TCOD_console_set_char_background(engine.get_console().get(), cx, cy, lightGrey, TCOD_BKGND_SET);
      }
    }
  }
  if (isPositionOK(mouseX, mouseY)) {
    TCOD_console_set_char_background(engine.get_console().get(), mouseX, mouseY, white, TCOD_BKGND_SET);
  }
}

bool Game::PickTile::isPositionOK(int x, int y) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  return game->map->isInFov(mouseX, mouseY) &&
         (maxRange == 0.0f || game->player->getDistance(mouseX, mouseY) <= maxRange);
}
