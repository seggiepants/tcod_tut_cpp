#include <cmath>
#include "MonsterAi.hpp"
#include "Engine.hpp"
#include "Actor.hpp"

static const int TRACKING_TURNS = 3;

void Game::MonsterAi::update(Actor* owner) {
    // Skip if actor is dead.
    if (owner->destructible && owner->destructible->isDead()) return;
    Game* game = (Game*)engine.scenes[GameScene::GAME];

    if (game->map->isInFov(owner->x, owner->y)) {
        // We can see the player, move toward them.
        moveCount = TRACKING_TURNS;
    } else if (moveCount > 0) {
        moveCount--;
    }

    if (moveCount > 0) {
        moveOrAttack(owner, game->player->x, game->player->y);
    }
}

void Game::MonsterAi::moveOrAttack(Actor* owner, int targetX, int targetY) {
    Game* game = (Game*)engine.scenes[GameScene::GAME];
    int dx = targetX - owner->x;
    int dy = targetY - owner->y;
    int stepdx = (dx > 0 ? 1: -1);
    int stepdy = (dy > 0 ? 1: -1);

    double distance = std::sqrt((dx * dx) + (dy * dy));

    if (distance >= 2.0) {
        // Normalize the vector.
        dx = (int)(round(dx/distance));
        dy = (int)(round(dy/distance));

        if (game->map->canWalk(owner->x + dx, owner->y + dy)) {
        owner->x += dx;
        owner->y += dy;
        } else if (game->map->canWalk(owner->x + stepdx, owner->y)) {
            // Wall slide - x
            owner->x += stepdx;
        } else if (game->map->canWalk(owner->x, owner->y + stepdy)) {
            // Wall slide - y
            owner->y += stepdy;
        }
    }
     else if (owner->attacker) { // we have an attacker object.
        owner->attacker->attack(owner, game->player);
    }
}

void Game::MonsterAi::load(std::ifstream& stream) {
    char delim = ',';
    // type already consumed
    stream >> moveCount >> delim;
}

void Game::MonsterAi::save(std::ofstream& stream) {
    const char delim = ',';
    stream << moveCount << delim;

}
