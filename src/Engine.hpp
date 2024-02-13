#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <list>
#include <filesystem>
#include <SDL.h>
#include <libtcod.hpp>
#include "Actor.hpp"
#include "Gui.hpp"
#include "Map.hpp"

class Engine {
public:
    Engine();
    ~Engine();
    void init(int argc, char** argv, int screenWidth, int screenHeight);
    void update();
    void render();
    bool isRunning() { return running; }
    void Stop() { running = false;}
    void sendToBack(Actor* actor);
    void flush() { context.present(console); }
    tcod::Console& getConsole() { return console; }; 

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
    Gui* gui;
    int fovRadius;
    SDL_Keycode currentKey;
    int get_width() { return screenWidth; }
    int get_height() { return screenHeight; }
    bool mouseClicked;
    int mouseX; // mouse position in tiles/characters
    int mouseY;
protected:
    std::filesystem::path GetDataDir();
    tcod::Console console;
    tcod::Context context;
    bool running;
    bool initialized;
    int screenWidth;
    int screenHeight;
    int tileWidth;
    int tileHeight;
};
extern TCOD_ColorRGBA BLACK;
extern TCOD_ColorRGBA BLUE;
extern TCOD_ColorRGBA CYAN;
extern TCOD_ColorRGBA GREEN;
extern TCOD_ColorRGBA MAGENTA;
extern TCOD_ColorRGBA RED;
extern TCOD_ColorRGBA WHITE;
extern TCOD_ColorRGBA VIOLET;
extern TCOD_ColorRGBA YELLOW;
extern TCOD_ColorRGBA COLOR_CORPSE;
extern TCOD_ColorRGBA COLOR_ORC;
extern TCOD_ColorRGBA COLOR_TROLL;
extern TCOD_ColorRGBA DARK_GROUND;
extern TCOD_ColorRGBA DARK_WALL;
extern TCOD_ColorRGBA LIGHT_GROUND;
extern TCOD_ColorRGBA LIGHT_WALL;
extern TCOD_ColorRGBA UNEXPLORED;

extern TCOD_ColorRGB black;
extern TCOD_ColorRGB darkerRed;
extern TCOD_ColorRGB lightGrey;
extern TCOD_ColorRGB lightRed;
extern TCOD_ColorRGB red;
extern TCOD_ColorRGB white;
extern Engine engine;

#endif