#include "PlayerAi.hpp"

#include <SDL.h>

#include "Actor.hpp"
#include "Container.hpp"
#include "Engine.hpp"
#include "Game.hpp"
#include "Pickable.hpp"
#include "Menu.hpp"

#ifdef _WIN32
#define UNUSED
#else
#define UNUSED __attribute__((unused))
#endif

const int LEVEL_UP_BASE = 200;
const int LEVEL_UP_FACTOR = 150;

Game::PlayerAi::PlayerAi() : xpLevel(1) {}

int Game::PlayerAi::getNextLevelXp() {
  return LEVEL_UP_BASE + (xpLevel * LEVEL_UP_FACTOR);
}

void Game::PlayerAi::update(Actor* owner) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];

  if (owner->destructible && owner->destructible->isDead()) {
    // Do nothing if dead.
    return;
  }

  int nextLevelUpXp = getNextLevelXp();
  if (owner->destructible && owner->destructible->xp >= nextLevelUpXp) {
    xpLevel++;
    owner->destructible->xp -= nextLevelUpXp;
    game->gui->message(yellow, "Your battle skills grow stronger! You reached level %d", xpLevel);

    Menu* next = (Menu*)engine.scenes[GameScene::MENU];
    next->mode = Menu::LEVELUP;
    next->init();
    engine.currentScene = next;
  }

  int dx = 0, dy = 0;

  switch (game->currentKey) {
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
    default:
      handleActionKey(owner, game->currentKey);
      break;
  }

  if (dx != 0 || dy != 0) {
    game->gameStatus = Game::NEW_TURN;
    if (moveOrAttack(owner, owner->x + dx, owner->y + dy)) {
      game->map->computeFov();
    }
  }
}

Game::Actor* Game::PlayerAi::chooseFromInventory(Actor* owner) {
  static const int INVENTORY_WIDTH = 50;
  static const int INVENTORY_HEIGHT = 28;  // 26 letters in alphabet + 2 for border.

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
  SDL_Event event;
  SDL_Keycode currentKey;
  bool done = false;
  while (!done) {
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
              return (*iter);
            }
          }
          done = true;
          break;
      }
    }
  }
  return nullptr;
}

void Game::PlayerAi::useInventory(Actor* owner, Actor* wearer) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  if (owner && owner->pickable) {
    owner->pickable->use(owner, wearer);
    game->gameStatus = Game::NEW_TURN;
  }
}

void Game::PlayerAi::dropInventory(Actor* owner, Actor* wearer) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  if (owner && owner->pickable) {
    owner->pickable->drop(owner, wearer);
    game->gameStatus = Game::NEW_TURN;
  }
}

void Game::PlayerAi::handleActionKey(Actor* owner, int key) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  switch (key) {
    case SDLK_g: {
      bool found = false;
      for (auto const& actor : game->actors) {
        if (actor->pickable && actor->x == owner->x && actor->y == owner->y) {
          Actor* picked = actor;
          if (actor->pickable->pick(actor, owner)) {
            found = true;
            game->gui->message(lightGrey, "You picked up the %s.", picked->name.c_str());
            break;
          } else if (!found) {
            found = true;
            game->gui->message(red, "Your inventory is full.");
          }
        }
      }
      if (!found) {
        game->gui->message(lightGrey, "There is nothing here that you can pick up.");
      }
    } break;
    case SDLK_i:
      engine.pickInventory(owner, InventoryCommand::USE);
      break;
    case SDLK_d:  // drop item
      engine.pickInventory(owner, InventoryCommand::DROP);
      break;
    case SDLK_PERIOD: // > and . are on the same key.
    case SDLK_GREATER: // >
    case SDLK_KP_GREATER: // > on numeric keypad?? Didn't know that existed.
      if (game->stairs->x == owner->x && game->stairs->y == owner->y) {
        game->nextLevel();
      } else {
        game->gui->message(lightGrey, "There are no stairs here.");
      }
      break;
    default:
      break;
  }
}

bool Game::PlayerAi::moveOrAttack(Actor* owner, int targetX, int targetY) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  if (game->map->isWall(targetX, targetY)) return false;

  // look for actors that are still alive to attack
  for (auto& actor : game->actors) {
    if (actor != owner &&  // Make sure we don't attack ourself.
        owner->attacker &&  // Make sure we have an attacker
        actor->destructible &&  // Make sure we are attacking something that can be destroyed
        !actor->destructible->isDead() &&  // and that it isn't already dead
        actor->x == targetX &&  // and it is where we are attacking.
        actor->y == targetY) {
      owner->attacker->attack(owner, actor);
      return false;  // exit after attacking so we don't also move
    }
  }

  // look for corpses
  for (auto const& actor : game->actors) {
    bool corpseOrItem = ((actor->destructible && actor->destructible->isDead()) || actor->pickable);
    if (corpseOrItem && actor->x == targetX && actor->y == targetY) {
      game->gui->message(lightGrey, "There's a %s here", actor->name.c_str());
      break;
    }
  }
  owner->x = targetX;
  owner->y = targetY;
  return true;
}

void Game::PlayerAi::load(std::ifstream& stream) {
  char delim = ',';

  stream >> xpLevel >> delim;
}

void Game::PlayerAi::save(std::ofstream& stream) {
  const char delim = ',';
  stream << xpLevel << delim;
}
