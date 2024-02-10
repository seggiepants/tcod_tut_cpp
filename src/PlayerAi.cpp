#include "PlayerAi.hpp"
#include "Engine.hpp"
#include <SDL.h>

void PlayerAi::update(Actor* owner) {
    if (owner->destructible && owner->destructible->isDead()) {
        // Do nothing if dead.
        return;
    }

    int dx = 0, dy = 0;

    switch(engine.currentKey) {
    case SDLK_UP:
    case SDLK_w:
        dy = -1;
        break;
    case SDLK_DOWN:
    case SDLK_s:
        dy = 1;
        break;
    case SDLK_LEFT:
    case SDLK_a:
        dx = -1;
        break;
    case SDLK_RIGHT:
    case SDLK_d:
        dx = 1;
        break;
    default:
        break;
    }
    
    if (dx!= 0 || dy != 0) {
        engine.gameStatus = Engine::NEW_TURN;
        if (moveOrAttack(owner, owner->x + dx, owner->y + dy )) {
            engine.map->computeFov();            
        }
    }    
}

bool PlayerAi::moveOrAttack(Actor* owner, int targetX, int targetY) {
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
        if (actor->destructible && actor->destructible->isDead() && actor->x == targetX && actor->y == targetY) {
            engine.gui->message(lightGrey, "There's a %s here", actor->name->c_str());
            break;
        }
    }
    owner->x = targetX;
    owner->y = targetY;
    return true;
}