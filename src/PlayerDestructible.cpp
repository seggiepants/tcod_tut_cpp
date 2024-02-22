#include "PlayerDestructible.hpp"

#include "Engine.hpp"

Game::PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char* corpseName, int xp)
    : Destructible(maxHp, defense, corpseName, xp) {}

void Game::PlayerDestructible::die(Actor* owner) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  game->gui->message(red, "You died!");
  Destructible::die(owner);
  game->gameStatus = Game::DEFEAT;
}

void Game::PlayerDestructible::save(std::ofstream& stream) { Destructible::save(stream); }
