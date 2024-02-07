#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <list>
#include <filesystem>
#include <libtcod.hpp>
#include "Actor.hpp"
#include "Map.hpp"

class Engine {
public:
    Engine();
    ~Engine();
    void init(int argc, char** argv);
    void update();
    void render();
    bool isRunning() { return running; }
    void tryMove(Actor* actor, int x, int y);

    std::list<Actor *> actors;
    enum GameStatus {
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT
    } gameStatus;
    Actor* player;
    Map* map;
    int fovRadius;
protected:
    std::filesystem::path GetDataDir();
    tcod::Console console;
    tcod::Context context;
    bool running;
    bool initialized;
};
extern TCOD_ColorRGBA BLACK;
extern TCOD_ColorRGBA BLUE;
extern TCOD_ColorRGBA CYAN;
extern TCOD_ColorRGBA GREEN;
extern TCOD_ColorRGBA MAGENTA;
extern TCOD_ColorRGBA RED;
extern TCOD_ColorRGBA WHITE;
extern TCOD_ColorRGBA YELLOW;
extern Engine engine;

#endif