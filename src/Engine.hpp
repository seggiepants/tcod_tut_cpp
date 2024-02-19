#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <list>
#include <filesystem>
#include <SDL.h>
#include <libtcod.hpp>
#include "Actor.hpp"
#include "Gui.hpp"
#include "Map.hpp"

namespace Game {
    class Engine {
    public:
        Engine();
        ~Engine();
        void preInit(int argc, char** argv, int screenWidth, int screenHeight);
        void destroy();
        void init();
        void load();
        void save();
        void update();
        void render();
        void present() {context.present(console);};
        Actor* getActor(int x, int y) const;
        bool isRunning() { return running; }
        bool pickATile(int* x, int* y, float maxRange = 0.0f);
        void Stop() { running = false;}
        void sendToBack(Actor* actor);
        void flush() { context.present(console); }
        tcod::Console& get_console() { return console; }; 
        Actor* getClosestMonster(int x, int y, float range) const;

        std::list<Game::Actor*> actors;

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
}
    
extern TCOD_ColorRGBA BLACK;
extern TCOD_ColorRGBA BLUE;
extern TCOD_ColorRGBA CYAN;
extern TCOD_ColorRGBA GREEN;
extern TCOD_ColorRGBA LIGHT_BLUE;
extern TCOD_ColorRGBA LIGHT_GREEN;
extern TCOD_ColorRGBA LIGHT_YELLOW;
extern TCOD_ColorRGBA MAGENTA;
extern TCOD_ColorRGBA ORANGE;
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
extern TCOD_ColorRGB cyan;
extern TCOD_ColorRGB darkerRed;
extern TCOD_ColorRGB lightBlue;
extern TCOD_ColorRGB lightGreen;
extern TCOD_ColorRGB lightGrey;
extern TCOD_ColorRGB lightRed;
extern TCOD_ColorRGB lightYellow;
extern TCOD_ColorRGB orange;
extern TCOD_ColorRGB red;
extern TCOD_ColorRGB white;
extern Game::Engine engine;


#endif