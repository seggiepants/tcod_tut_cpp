#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <list>
#include <filesystem>
#include <libtcod.hpp>
#include "Actor.hpp"
#include "Map.hpp"

class Engine {
    public:
    std::list<Actor *> actors;
    Actor* player;
    Map* map;

    Engine(int argc, char** argv);
    ~Engine();
    void update();
    void render();
    bool isRunning() { return running; }
    void tryMove(Actor* actor, int x, int y);
    protected:
    std::filesystem::path GetDataDir();
    tcod::Console console;
    tcod::Context context;
    bool running;
};
extern TCOD_ColorRGBA BLACK;
extern TCOD_ColorRGBA BLUE;
extern TCOD_ColorRGBA CYAN;
extern TCOD_ColorRGBA GREEN;
extern TCOD_ColorRGBA MAGENTA;
extern TCOD_ColorRGBA RED;
extern TCOD_ColorRGBA WHITE;
extern TCOD_ColorRGBA YELLOW;
extern Engine* engine;

#endif