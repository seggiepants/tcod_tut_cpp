#include "PlayerAi.hpp"
#include "Engine.hpp"
#include "Actor.hpp"
#include "Container.hpp"
#include "Pickable.hpp"
#include <SDL.h>

#ifdef _WIN32
#define UNUSED
#else
#define UNUSED __attribute__((unused))
#endif

void Game::PlayerAi::update(Actor* owner) {
    if (owner->destructible && owner->destructible->isDead()) {
        // Do nothing if dead.
        return;
    }

    int dx = 0, dy = 0;

    switch(engine.currentKey) {
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
        handleActionKey(owner, engine.currentKey);
        break;
    }
    
    if (dx!= 0 || dy != 0) {
        engine.gameStatus = Engine::NEW_TURN;
        if (moveOrAttack(owner, owner->x + dx, owner->y + dy )) {
            engine.map->computeFov();            
        }
    }    
}

Game::Actor* Game::PlayerAi::chooseFromInventory(Actor* owner) {
    static const int INVENTORY_WIDTH = 50;
    static const int INVENTORY_HEIGHT = 28; // 26 letters in alphabet + 2 for border.
    
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
    for(auto const & actor : owner->container->inventory) {
        TCOD_console_printf(con.get(), 2, y, "(%c) %s", shortcut, actor->name.c_str());
        y++;
        shortcut++;
    }    
    tcod::blit(engine.get_console(), con, {(engine.get_console().get_width() - INVENTORY_WIDTH) / 2, (engine.get_console().get_height() - INVENTORY_HEIGHT) / 2},{0, 0, con.get_width(), con.get_height()}, 1.0f, 1.0f);    
    engine.flush();
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
                if (currentKey >= SDLK_a && currentKey <= SDLK_z)
                {
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

void Game::PlayerAi::handleActionKey(Actor* owner, int key) {
    switch(key) {
    case SDLK_g:
        {
            bool found = false;
            for(auto const & actor : engine.actors){
                if (actor->pickable && actor->x == owner->x && actor->y == owner-> y) {
                    if (actor->pickable->pick(actor, owner)) {
                        found = true;
                        engine.gui->message(lightGrey, "You picked up the %s.", actor->name.c_str());
                        break;
                    } else if (!found) {
                        found = true;
                        engine.gui->message(red, "Your inventory is full.");
                    }
                }
            }
            if (!found) {
                engine.gui->message(lightGrey, "There is nothing here that you can pick up.");
            }
        }
        break;
    case SDLK_i:
        {
            Actor* actor = chooseFromInventory(owner);
            if (actor) {
                actor->pickable->use(actor, owner);
                engine.gameStatus = Engine::NEW_TURN;
            }
        }
        break;
    case SDLK_d: // drop item
        {
            Actor* actor = chooseFromInventory(owner);
            if (actor && actor->pickable) {
                actor->pickable->drop(actor, owner);
                engine.gameStatus = Engine::NEW_TURN;
            }
        }
        break;
    default:
        break;
    }
}

bool Game::PlayerAi::moveOrAttack(Actor* owner, int targetX, int targetY) {
    if (engine.map->isWall(targetX, targetY)) return false;

    // look for actors that are still alive to attack
    for (auto & actor : engine.actors) {
        if (actor != owner &&  // Make sure we don't attack ourself.
            owner->attacker && // Make sure we have an attacker
            actor->destructible &&  // Make sure we are attacking something that can be destroyed
            !actor->destructible->isDead() && // and that it isn't already dead
            actor->x == targetX && // and it is where we are attacking.
            actor->y == targetY) {
            owner->attacker->attack(owner, actor);
            return false; // exit after attacking so we don't also move
        }
    }

    // look for corpses
    for (auto const & actor : engine.actors) {
        bool corpseOrItem = ((actor->destructible && actor->destructible->isDead()) || actor->pickable);
        if (corpseOrItem && actor->x == targetX && actor->y == targetY) {
            engine.gui->message(lightGrey, "There's a %s here", actor->name.c_str());
            break;
        }
    }
    owner->x = targetX;
    owner->y = targetY;
    return true;
}

void Game::PlayerAi::load(UNUSED std::ifstream& stream) {
    // Type already read previously no data left.
    return;
}

void Game::PlayerAi::save(UNUSED std::ofstream& stream) {
    return;
}

