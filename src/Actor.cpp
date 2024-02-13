#include "Actor.hpp"
#include "Engine.hpp"
#include "Container.hpp"
#include "Pickable.hpp"

Actor::Actor(int x, int y, int ch, const char* name, const TCOD_ColorRGBA & col) :
x(x), y(y), ch(ch), col(col), blocks(true), attacker(nullptr), destructible(nullptr), ai(nullptr),
pickable(nullptr), container(nullptr) {
    this->name = new std::string(name);
}

Actor::~Actor() {
    delete this->name;
    if (attacker) delete attacker;
    if (destructible) delete destructible;
    if (ai) delete ai;
    if (pickable) delete pickable;
    if (container) delete container;
}

void Actor::render(tcod::Console & console) const {
    TCOD_ConsoleTile& tile = console.at(x, y);
    tile.ch = ch;
    tile.fg = col;    
}

void Actor::update() {
    if (ai) ai->update(this);
}