#include "Actor.hpp"
#include "Engine.hpp"

Actor::Actor(int x, int y, int ch, const char* name, const TCOD_ColorRGBA & col) :
x(x), y(y), ch(ch), col(col), blocks(true), attacker(nullptr), destructible(nullptr), ai(nullptr) {
    this->name = new std::string(name);
}

Actor::~Actor() {
    delete this->name;
}

void Actor::render(tcod::Console & console) const {
    TCOD_ConsoleTile& tile = console.at(x, y);
    tile.ch = ch;
    tile.fg = col;    
}

void Actor::update() {
    if (ai) ai->update(this);
}