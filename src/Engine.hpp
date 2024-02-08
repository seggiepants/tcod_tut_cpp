#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <list>
#include <filesystem>
#include <SDL.h>
#include <libtcod.hpp>
#include "Actor.hpp"
#include "Map.hpp"

class Engine {
public:
    Engine();
    ~Engine();
    void init(int argc, char** argv, int screenWidth, int screenHeight);
    void update();
    void render();
    bool isRunning() { return running; }
    void sendToBack(Actor* actor);

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
    SDL_Keycode currentKey;
protected:
    std::filesystem::path GetDataDir();
    tcod::Console console;
    tcod::Context context;
    bool running;
    bool initialized;
    int screenWidth;
    int screenHeight;
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