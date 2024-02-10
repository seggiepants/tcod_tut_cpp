#include <iostream>
#include <SDL.h>
#include "Engine.hpp"
#include "Attacker.hpp"
#include "PlayerDestructible.hpp"
#include "PlayerAi.hpp"

TCOD_ColorRGBA BLACK = {0, 0, 0, 255};
TCOD_ColorRGBA BLUE = {0, 0, 255, 255};
TCOD_ColorRGBA CYAN = {0, 255, 255, 255};
TCOD_ColorRGBA GREEN = {0, 255, 0, 255};
TCOD_ColorRGBA MAGENTA = {255, 0, 255, 255};
TCOD_ColorRGBA RED = {255, 0, 0, 255};
TCOD_ColorRGBA WHITE = {255, 255, 255, 255};
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
TCOD_ColorRGB darkerRed = {127, 0, 0}; 
TCOD_ColorRGB lightGrey = {159, 159, 159};
TCOD_ColorRGB lightRed = {255, 63, 63}; 
TCOD_ColorRGB red = {255, 0, 0};
TCOD_ColorRGB white = {255, 255, 255};

Engine::Engine() : gameStatus(STARTUP), fovRadius(10), currentKey((SDL_Keycode)0) {
    map = nullptr;
    gui = nullptr;
    initialized = false;
}

Engine::~Engine() {
    for(auto const & actor : actors){
        delete actor;
    }
    actors.clear();
    if (map) delete map;
    if (gui) delete gui;
}

void Engine::init(int argc, char** argv, int screenWidth, int screenHeight) {
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

        auto tileset = tcod::load_tilesheet(GetDataDir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
        params.tileset = tileset.get();
        tileWidth = tileset.get_tile_height();
        tileHeight = tileset.get_tile_width();

        console = tcod::Console{screenWidth, screenHeight};
        params.console = console.get();

        context = tcod::Context(params);    
        player = new Actor(console.get_width() / 2, console.get_height() / 2, '@', "player", WHITE);
        player->destructible = new PlayerDestructible(30, 2, "your cadaver");
        player->attacker = new Attacker(5);
        player->ai = new PlayerAi();
        actors.push_back(player);
        gui = new Gui();
        map = new Map(console.get_width(), console.get_height() - gui->get_height());
        gui->message(red, "Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.");
        initialized = true;
        gameStatus = STARTUP;
        running = true;
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << "\n";
        throw;
    }
}

void Engine::update() {
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
            running = false;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
            case SDLK_ESCAPE:
                running = false;
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

void Engine::render() {
    console.clear();
    map->render(console);
    for(auto const & actor : actors) {
        if (map->isInFov(actor->x, actor->y)) {
            actor->render(console);
        }
    }

    // start of a gui
    /*
    if (player->destructible) {
        TCOD_console_printf(console.get(), 1, console.get_height() - 2, "HP: %d/%d",(int)player->destructible->hp, (int)player->destructible->maxHp);
    }
    */
    gui->render(console);
    context.present(console);
}

void Engine::sendToBack(Actor* actor) {
    actors.remove(actor);
    actors.push_front(actor);
}

/// Return the data directory.
std::filesystem::path Engine::GetDataDir() {
    static auto root_directory = std::filesystem::path{"."};  // Begin at the working directory.
    while (!std::filesystem::exists(root_directory / "data")) {
        // If the current working directory is missing the data dir then it will assume it exists in any parent directory.
        root_directory /= "..";
        if (!std::filesystem::exists(root_directory)) {
            throw std::runtime_error("Could not find the data directory.");
        }
    }
    return root_directory / "data";
};
