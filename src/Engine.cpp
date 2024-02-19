#include <fstream>
#include <iostream>
#include <SDL.h>
#include "Engine.hpp"
#include "Attacker.hpp"
#include "PlayerDestructible.hpp"
#include "PlayerAi.hpp"
#include "Container.hpp"

TCOD_ColorRGBA BLACK = {0, 0, 0, 255};
TCOD_ColorRGBA BLUE = {0, 0, 255, 255};
TCOD_ColorRGBA CYAN = {0, 255, 255, 255};
TCOD_ColorRGBA GREEN = {0, 255, 0, 255};
TCOD_ColorRGBA LIGHT_BLUE = {63, 63, 255, 255};
TCOD_ColorRGBA LIGHT_GREEN = {63, 255, 63, 255};
TCOD_ColorRGBA LIGHT_YELLOW = {255, 255, 63, 255};
TCOD_ColorRGBA LIGHTER_ORANGE = {255, 191, 127, 255};
TCOD_ColorRGBA MAGENTA = {255, 0, 255, 255};
TCOD_ColorRGBA ORANGE = {255, 127, 0, 255};
TCOD_ColorRGBA RED = {255, 0, 0, 255};
TCOD_ColorRGBA WHITE = {255, 255, 255, 255};
TCOD_ColorRGBA VIOLET = {127, 0, 255, 255};
TCOD_ColorRGBA YELLOW = {255, 255, 0, 255};
TCOD_ColorRGBA COLOR_CORPSE = {191, 0, 0, 255};
TCOD_ColorRGBA COLOR_ORC = {63, 127, 63, 255};
TCOD_ColorRGBA COLOR_TROLL = {0, 127, 0, 255};
TCOD_ColorRGBA DARK_GROUND = {50, 50, 150, 255};    
TCOD_ColorRGBA DARK_WALL = {0, 0, 100, 255};
TCOD_ColorRGBA LIGHT_GROUND = {200, 180, 50, 255 };
TCOD_ColorRGBA LIGHT_WALL = {130, 110, 50, 255 };
TCOD_ColorRGBA UNEXPLORED = {0, 0, 0, 255};

TCOD_ColorRGB black = {0, 0, 0};
TCOD_ColorRGB cyan = {0, 255, 255};
TCOD_ColorRGB darkerRed = {127, 0, 0}; 
TCOD_ColorRGB lightBlue = {63, 63, 255};
TCOD_ColorRGB lightGreen = {63, 255, 63};
TCOD_ColorRGB lightGrey = {159, 159, 159};
TCOD_ColorRGB lightRed = {255, 63, 63};
TCOD_ColorRGB lightYellow = {255, 255, 63};
TCOD_ColorRGB lighterOrange = {255, 191, 127};
TCOD_ColorRGB orange = {255, 127, 0};
TCOD_ColorRGB red = {255, 0, 0};
TCOD_ColorRGB white = {255, 255, 255};

const char* SAVE_FILENAME = "game.sav";


Game::Engine::Engine() : gameStatus(STARTUP), fovRadius(10), currentKey((SDL_Keycode)0) {
    player = nullptr;
    map = nullptr;
    gui = nullptr;
    initialized = false;
}

Game::Engine::~Engine() {
    destroy();
    delete gui;
    gui = nullptr;
}

void Game::Engine::destroy() {
    for(auto& actor : actors) {
        delete actor;
    }
    actors.clear();    
    player = nullptr;

    if (map) {
        delete map;
        map = nullptr;
    }

    if (gui) {
        gui->clear();
    }
}

void Game::Engine::preInit(int argc, char** argv, int screenWidth, int screenHeight) {
if (initialized)
        return;

    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    
    try {
        auto params = TCOD_ContextParams{};
        params.tcod_version = TCOD_COMPILEDVERSION;
        params.argc = argc;
        params.argv = argv;
        params.renderer_type = TCOD_RENDERER_SDL2;
        params.vsync = 1;
        params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
        params.window_title = "Libtcod C++ Tutorial";

        auto tileset = tcod::load_tilesheet(GetRootDir() / "data" / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
        params.tileset = tileset.get();
        tileWidth = tileset.get_tile_height();
        tileHeight = tileset.get_tile_width();

        console = tcod::Console{screenWidth, screenHeight};
        params.console = console.get();

        context = tcod::Context(params);   
        gameStatus = STARTUP;
        running = false;
        initialized = true;
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << "\n";
        throw;
    }
}

void Game::Engine::init() {
    destroy(); // Make sure everything is clean

    player = new Actor(console.get_width() / 2, console.get_height() / 2, '@', "player", WHITE);
    player->destructible = new PlayerDestructible(30, 2, "your cadaver");
    player->attacker = new Attacker(5);
    player->ai = new PlayerAi();
    player->container = new Container(26);
    if (!gui)
        gui = new Gui();

    map = new Map(console.get_width(), console.get_height() - gui->get_height());
    map->init(true);
    actors.push_back(player); // Make sure player is on-top.
    gui->message(red, "Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.");
    gameStatus = GameStatus::STARTUP;
    running = true;
}

void Game::Engine::load() {
    bool hasSaveFile = false;

#ifndef __EMSCRIPTEN__
    std::filesystem::path path(SAVE_FILENAME);
    if (std::filesystem::exists(path))  {
        hasSaveFile = true;
    }
#endif

    menu.clear();
    menu.addItem(Menu::NEW_GAME, "New Game");
    if (hasSaveFile) {
        menu.addItem(Menu::CONTINUE, "Continue");
    }
    menu.addItem(Menu::EXIT, "Exit");

    Menu::MenuItemCode selection = menu.pick();

    if (selection == Menu::EXIT || selection == Menu::NONE) {
        Stop();
        return;
    } else if (selection == Menu::NEW_GAME) {
        destroy();
        init();
    } else { // selection == MenuItemCode::CONTINUE 
        std::ifstream stream(SAVE_FILENAME, std::ios::binary | std::ios::in);
        // cereal::JSONInputArchive archive(fs);

        destroy();

        // load the map
        int width, height;
        // archive(width, height);
        char delim = ',';
        stream >> width >> delim >> height >> delim;
        map = new Map(width, height);
        map->width = width;
        map->height = height;
        map->init(false);
        gui = new Gui();
        map = new Map(console.get_width(), console.get_height() - gui->get_height());
        map->load(stream);
        
        int actorCount = 0;
        stream >> actorCount >> delim;
        for(int i = 0; i < actorCount; ++i) {
            Actor* currentActor = new Actor();
            currentActor->load(stream);
            stream >> delim >> delim;
            actors.push_back(currentActor);
        }
        
        // reference the player
        for(auto const & actor: actors) {
            if (actor->ch == (int) '@') {
                player = actor;
                break;
            }
        }

        gui->load(stream);

        stream.close();
        gameStatus = GameStatus::STARTUP;
        running = true;        
    }
}

void Game::Engine::save() {
#ifndef __EMSCRIPTEN__    
    const char delim = ',';
    if (!player) return;
    if (!player->destructible) return;    

    if (player->destructible->isDead()) {
        std::filesystem::path path(SAVE_FILENAME);
        if (std::filesystem::exists(path)) {
            std::filesystem::remove(path);
        }
    } else {
        std::ofstream stream(SAVE_FILENAME, std::ios::binary | std::ios::out);

        // save the map first
        stream << map->width << delim << map->height << delim;
        map->save(stream);    
        
        //archive(actors);
        
        stream << actors.size() << delim;
        for(auto const& actor : actors) {
            actor->save(stream);
            stream << '~' << delim;
        }
        
        gui->save(stream);

        stream.flush();
        stream.close();
    }
#endif
}

void Game::Engine::update() {
    // Handle input.
    SDL_Event event;
    currentKey = (SDL_Keycode)0; // Clear each time  only new keys stay
    mouseClicked = false; // Clear each time only want current status for the frame.

    if (gameStatus == STARTUP) {
        // Compute FOV on first frame.
        map->computeFov();
    }
    gameStatus = IDLE;

#ifndef __EMSCRIPTEN__
    // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
    SDL_WaitEvent(nullptr);
#endif
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            //std::exit(EXIT_SUCCESS);
            Stop();
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
            case SDLK_ESCAPE:
#ifndef __EMSCRIPTEN__
                save();
                load();
#endif
                break;
            default:
                currentKey = event.key.keysym.sym;
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
            case SDL_BUTTON_RIGHT:
                // Split this out later - left move, right look

                break;
            }
            break;
        case SDL_MOUSEMOTION:
            mouseX = event.motion.x / tileWidth;
            mouseY = event.motion.y / tileHeight;
        }
    }

    player->update();
    if (gameStatus == NEW_TURN) {
        for(auto const & actor : actors) {
            if (actor != player) {
                actor->update();
            }
        }
    }
}

void Game::Engine::render() {
    console.clear();
    map->render();
    for(auto const & actor : actors) {
        if (map->isInFov(actor->x, actor->y)) {
            actor->render();
        }
    }

    // start of a gui
    /*
    if (player->destructible) {
        TCOD_console_printf(console.get(), 1, console.get_height() - 2, "HP: %d/%d",(int)player->destructible->hp, (int)player->destructible->maxHp);
    }
    */
    gui->render();    
}

bool Game::Engine::pickATile(int* x, int* y, float maxRange) {
    SDL_Event event;
    int mouseX = player->x; // This might fail on chromebook.
    int mouseY = player->y; // going to add cursor keys as a fall-back.
    int dx = 0;
    int dy = 0;
    bool positionOK = false;

    while (true) {
        render();

        // highlight the possible range
        for(int cy = 0; cy < map->height; ++cy) {
            for (int cx = 0; cx < map->width; ++cx) {
                if (map->isInFov(cx, cy) && (maxRange == 0.0f || player->getDistance(cx, cy) < maxRange)) {
                    /*TCOD_ColorRGB clr = TCOD_console_get_char_background(console.get(), cx, cy);
                    clr.r *= brighten;
                    clr.g *= brighten;
                    clr.b *= brighten;
                    */
                    TCOD_console_set_char_background(console.get(), cx, cy, lightGrey, TCOD_BKGND_SET);
                }
            }
        }        
        positionOK = map->isInFov(mouseX, mouseY) && (maxRange == 0.0f || player->getDistance(mouseX, mouseY) <= maxRange);
        if (positionOK) {
            //TCOD_console_printf(console.get(),0, 0, "Picking at (%d,%d)...", mouseX, mouseY);
            TCOD_console_set_char_background(console.get(), mouseX, mouseY, white, TCOD_BKGND_SET);
            // TCOD_console_set_char(console.get(), mouseX, mouseY, (int) 'X');
        }

        // Wait for a key up or exit
        while (SDL_PollEvent(&event)) {
            dx = 0;
            dy = 0;
            switch (event.type) {
            case SDL_QUIT:
                engine.Stop();
                return false;
                break;
            case SDL_KEYDOWN:
                {
                    currentKey = event.key.keysym.sym;
                    switch(engine.currentKey) {
                    case SDLK_ESCAPE:
                        return false;
                        break;
                    case SDLK_RETURN:
                    case SDLK_RETURN2:
                    case SDLK_KP_ENTER:
                        *x = mouseX;
                        *y = mouseY;
                        return positionOK;
                        break;
                    case SDLK_KP_9:
                        dy = -1;
                        dx = 1;
                        break;
                    case SDLK_UP:
                    case SDLK_KP_8:
                        dy = -1;
                        break;
                    case SDLK_KP_7:
                        dy = -1;
                        dx = -1;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_KP_6:
                        dx = 1;
                        break;
                    case SDLK_LEFT:
                    case SDLK_KP_4:
                        dx = -1;
                        break;
                    case SDLK_KP_3:
                        dy = 1;
                        dx = 1;
                        break;
                    case SDLK_DOWN:
                    case SDLK_KP_2:
                        dy = 1;
                        break;
                    case SDLK_KP_1:
                        dy = 1;
                        dx = -1;
                        break;                        
                    }
                    
                    if (dx!= 0 || dy != 0) {
                        mouseX += dx;
                        mouseY += dy;
                    }
                }    
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    switch (event.button.button)
                    {
                    case SDL_BUTTON_LEFT:
                        *x = mouseX;
                        *y = mouseY;
                        return positionOK;
                        break;
                    case SDL_BUTTON_RIGHT:
                        return false;
                        break;
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x / tileWidth;
                mouseY = event.motion.y / tileHeight;
                break;
            }
        }
        flush();
    }
    return false;
}

void Game::Engine::sendToBack(Actor* actor) {
    actors.remove(actor);
    actors.push_front(actor);
}

Game::Actor* Game::Engine::getClosestMonster(int x, int y, float range) const {
    Actor* closest = nullptr;

    float bestDistance = 1E6f; // start with a really big number.
    
    for(auto const & actor : actors){
        if (actor != player && actor->destructible && !actor->destructible->isDead()) {
            float distance = actor->getDistance(x, y);
            if (distance < bestDistance && (distance <= range || range == 0.0f)) {
                bestDistance = distance;
                closest = actor;
            }
        }
    }

    return closest;
}

Game::Actor* Game::Engine::getActor(int x, int y) const {
    for(auto const & actor : actors){
        if (actor->x == x && actor->y == y && actor->destructible && !actor->destructible->isDead()) {
            return actor;
        }
    }
    return nullptr;
}

/// Return the data directory.
std::filesystem::path Game::Engine::GetRootDir() {
    static auto root_directory = std::filesystem::path{"."};  // Begin at the working directory.
    while (!std::filesystem::exists(root_directory / "data")) {
        // If the current working directory is missing the data dir then it will assume it exists in any parent directory.
        root_directory /= "..";
        if (!std::filesystem::exists(root_directory)) {
            throw std::runtime_error("Could not find the data directory.");
        }
    }
    return root_directory;
};
