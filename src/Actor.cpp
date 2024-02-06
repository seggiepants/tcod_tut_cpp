#include "Actor.hpp"

Actor::Actor(int x, int y, int ch, const TCOD_ColorRGBA & col) :
x(x), y(y), ch(ch), col(col) {}

void Actor::render(tcod::Console & console) const {
    TCOD_ConsoleTile& tile = console.at(x, y);
    tile.ch = ch;
    tile.fg = col;    
}