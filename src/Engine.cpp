#include <iostream>
#include <SDL.h>
#include "Engine.hpp"

TCOD_ColorRGBA BLACK = {0, 0, 0, 255};
TCOD_ColorRGBA BLUE = {0, 0, 255, 255};
TCOD_ColorRGBA CYAN = {0, 255, 255, 255};
TCOD_ColorRGBA GREEN = {0, 255, 0, 255};
TCOD_ColorRGBA MAGENTA = {255, 0, 255, 255};
TCOD_ColorRGBA RED = {255, 0, 0, 255};
TCOD_ColorRGBA WHITE = {255, 255, 255, 255};
TCOD_ColorRGBA YELLOW = {255, 255, 0, 255};

Engine::Engine() : gameStatus(STARTUP), fovRadius(10) {
    initialized = false;
}

Engine::~Engine() {
    for(auto const & actor : actors){
        delete actor;
    }
    actors.clear();
    delete map;
}

void Engine::init(int argc, char** argv) {
    if (initialized)
        return;
    
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

        console = tcod::Console{80, 40};
        params.console = console.get();

        context = tcod::Context(params);    
        player = new Actor(console.get_width() / 2, console.get_height() / 2, '@', "Player", WHITE);
        actors.push_back(player);
        map = new Map(console.get_width(), console.get_height());
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
    int dx = 0, dy = 0;

    if (gameStatus == STARTUP) {
        // Compute FOV on first frame.
        map->computeFov();
        // Then set to IDLE.
        gameStatus = IDLE;
    }

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
        case SDLK_UP:
        case SDLK_w:
            dy = -1;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            dy = 1;
            break;
        case SDLK_LEFT:
        case SDLK_a:
            dx = -1;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            dx = 1;
            break;
        }
        break;
    }

    if (dx!= 0 || dy != 0) {
        gameStatus = NEW_TURN;
        if (player->moveOrAttack(player->x + dx, player->y + dy )) {
            map->computeFov();
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
    context.present(console);
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
