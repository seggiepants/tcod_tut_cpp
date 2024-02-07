#include "Actor.hpp"
#include "Engine.hpp"
#include <iostream> // Remove when we get rid of debug messages.

Actor::Actor(int x, int y, int ch, const char* name, const TCOD_ColorRGBA & col) :
x(x), y(y), ch(ch), col(col) {
    this->name = name;
}

void Actor::render(tcod::Console & console) const {
    TCOD_ConsoleTile& tile = console.at(x, y);
    tile.ch = ch;
    tile.fg = col;    
}

bool Actor::moveOrAttack(int x, int y) {
    // Don't go out of map bounds.
    if (x < 0 || x >= engine.map->width || y < 0 || y >= engine.map->height)
        return false;
    
    // Can't walk into a wall.
    if (engine.map->isWall(x, y))
        return false;

    for(auto const & actor : engine.actors) {
        if (actor->x == x && actor->y == y) {
            std::cout << "The " << actor->name << " laughs at your puny efforts to attack him!" << std::endl;
            return false;
        }
    }
    this->x = x;
    this->y = y;
    return true;
}

void Actor::update() {
    std::cout << "The " << name << "growls" << std::endl;
}