#include "Destructible.hpp"
#include "Actor.hpp"
#include "Engine.hpp"
#include <string>

#include "MonsterDestructible.hpp"
#include "PlayerDestructible.hpp"

Game::Destructible::Destructible() : maxHp(0), hp(0), defense(0.0f) {
    corpseName.assign("");
}

Game::Destructible::Destructible(float maxHp, float defense, const char* corpseName) :
    maxHp(maxHp),
    hp(maxHp),
    defense(defense) {
        this->corpseName.assign(corpseName == nullptr ? "" : corpseName);
}

Game::Destructible::~Destructible() {
    corpseName.clear();
}

float Game::Destructible::heal(float amount) {
    hp += amount;
    if (hp > maxHp) {
        amount -= hp - maxHp;
        hp = maxHp;
    }
    return amount;
}

float Game::Destructible::takeDamage(Actor* owner, float damage) {
    damage -= defense;
    if (damage > 0) {
        hp -= damage;
        if (hp <= 0) {
            die(owner);
        }
    } else {
        damage = 0;
    }
    return damage;
}

void Game::Destructible::die(Actor* owner) {
    Game* game = (Game*)engine.scenes[GameScene::GAME];
    // transform the actor into a corpse!
    owner->ch = '%';
    owner->col = COLOR_CORPSE;
    owner->name.clear();
    owner->name.assign(corpseName.c_str()); 
    owner->blocks = false;
    // Make sure corpses are drawn before living actors
    for(auto const & actor : game->actors) {
        if (actor == owner) {
            engine.sendToBack(actor);
            break;
        }
    }
}

Game::Destructible* Game::Destructible::create(int type) {
    switch((DestructibleType)type) {
        case MONSTER: 
            return new MonsterDestructible(0.0f, 0.0f, ""); 
            break;
        case PLAYER: 
            return new PlayerDestructible(0.0f, 0.0f, ""); 
            break;
    }
    return nullptr;
}

void Game::Destructible::load(std::ifstream& stream) {
    char delim = ',';
    int bufferSize;
    stream >> maxHp >> delim >> hp >> delim >> defense >> delim >> bufferSize >> delim;
    char* buffer = new char[bufferSize + 1];
    stream.read(buffer, bufferSize);
    buffer[bufferSize] = '\0';
    corpseName.assign(buffer);
    delete[] buffer;
    stream >> delim;
}

void Game::Destructible::save(std::ofstream& stream) {
    const char delim = ',';
    stream << maxHp << delim << hp << delim << defense << delim << corpseName.size() << delim << corpseName.c_str() << delim;
}
