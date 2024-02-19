#include <cmath>
#include "Actor.hpp"
#include "Engine.hpp"

#include <iostream>

#include "Ai.hpp"
#include "Pickable.hpp"
#include "Container.hpp"

Game::Actor::Actor() :
x(0), y(0), ch(0), col(WHITE), blocks(true), attacker(nullptr), destructible(nullptr), ai(nullptr),
pickable(nullptr), container(nullptr) {
    this->name.assign("");
}

Game::Actor::Actor(int x, int y, int ch, const char* name, const TCOD_ColorRGBA & col) :
x(x), y(y), ch(ch), col(col), blocks(true), attacker(nullptr), destructible(nullptr), ai(nullptr),
pickable(nullptr), container(nullptr) {
    if (name != nullptr) {
        this->name.assign(name);
    }
}

Game::Actor::~Actor() {
    name.clear();
    if (attacker != nullptr) delete attacker;
    if (destructible != nullptr) delete destructible;
    if (ai != nullptr) delete ai;
    if (pickable != nullptr) delete pickable;
    if (container != nullptr) delete container;
}

void Game::Actor::render() const {
    tcod::Console& console = engine.get_console();
    TCOD_ConsoleTile& tile = console.at(x, y);
    tile.ch = ch;
    tile.fg = col;    
}

void Game::Actor::update() {
    if (ai) ai->update(this);
}

float Game::Actor::getDistance(int cx, int cy) const {
    int dx = x - cx;
    int dy = y - cy;
    return (float)sqrt((dx * dx) + (dy * dy));
}

void Game::Actor::load(std::ifstream& stream) {
    char delim = ',';
    int bufferSize;
    stream >> x >> delim >> y >> delim >> ch >> delim >> col.r >> delim >> col.g >> delim >> col.b >> delim;
    stream >> bufferSize >> delim;
    char* buffer = new char[bufferSize + 1];
    stream.read(buffer, bufferSize);
    buffer[bufferSize] = '\0';
    name.assign(buffer);    
    delete[] buffer;
    stream >> delim >> blocks >> delim;

    //attacker, destructible, ai, pickable, container);
    bool hasField = false;
    stream >> hasField >> delim;
    if (hasField) {                
        attacker = new Attacker(0.0f);
        attacker->load(stream);
    } else {attacker = nullptr;}

    hasField = false;
    stream >> hasField >> delim;
    if (hasField) {
        int destructibleType = 0;
        stream >> destructibleType >> delim;
        destructible = Destructible::create(destructibleType);
        destructible->load(stream);
    } else {destructible = nullptr;}

    hasField = false;
    stream >> hasField >> delim;
    if (hasField) {
        int aiType = 0;
        stream >> aiType >> delim;
        ai = Ai::create(aiType);
        ai->load(stream);
    } else {ai = nullptr;}

    hasField = false;
    stream >> hasField >> delim;
    if (hasField) {
        int pickableType = 0;
        stream >> pickableType >> delim;
        pickable = Game::Pickable::create(pickableType);
        pickable->load(stream);
    } else {pickable = nullptr;}
    
    hasField = false;
    stream >> hasField >> delim;
    if (hasField) {   
        container = new Container();             
        container->load(stream);
    } else {container = nullptr;}
}

void Game::Actor::save(std::ofstream& stream) {
    const char delim = ',';
    bool hasField = false;
    stream << x << delim << y << delim << ch << delim << col.r << delim << col.g << delim << col.b << delim;
    stream << name.size() << delim << name.c_str() << delim << blocks << delim;
    //attacker, destructible, ai, pickable, container);
    hasField = attacker;
    stream << hasField << delim;
    if (hasField)
        attacker->save(stream);

    hasField = destructible;
    stream << hasField << delim;
    if (hasField) {
        stream << destructible->getType() << delim;
        destructible->save(stream);
    }
    
    hasField = ai;
    stream << hasField << delim;
    if (hasField) {
        stream << ai->getType() << delim;
        ai->save(stream);
    }

    hasField = pickable;
    stream << hasField << delim;
    if (hasField) {
        stream << pickable->getType() << delim;
        pickable->save(stream);
    }
    
    hasField = container;
    stream << hasField << delim;
    if (hasField) {
        container->save(stream);
    }   
}