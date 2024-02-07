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

Engine::Engine() {
    try {
    auto params = TCOD_ContextParams{};
    params.tcod_version = TCOD_COMPILEDVERSION;
    //params.argc = argc;
    //params.argv = argv;
    params.renderer_type = TCOD_RENDERER_SDL2;
    params.vsync = 1;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.window_title = "Libtcod C++ Tutorial";

    auto tileset = tcod::load_tilesheet(GetDataDir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();

    console = tcod::Console{80, 40};
    params.console = console.get();

    context = tcod::Context(params);    
    player = new Actor(console.get_width() / 2, console.get_height() / 2, '@', WHITE);
    actors.push_back(player);
    map = new Map(console.get_width(), console.get_height());

    running = true;
    } catch (const std::exception& exc) {
    std::cerr << exc.what() << "\n";
    throw;
  }
}

Engine::~Engine() {
    for(auto const & actor : actors){
        delete actor;
    }
    actors.clear();
    delete map;
}

void Engine::update() {
    // Handle input.
    SDL_Event event;
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
            tryMove(player, player->x, player->y - 1);
            break;
        case SDLK_DOWN:
        case SDLK_s:
            tryMove(player, player->x, player->y + 1);
            break;
        case SDLK_LEFT:
        case SDLK_a:
            tryMove(player, player->x - 1, player->y);
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            tryMove(player, player->x + 1, player->y);
            break;
        case SDLK_F10:
#ifndef __EMSCRIPTEN__
            std::stringstream stream;
            bool newFile = false;
            int counter = 0;
            while (!newFile)
            {
                counter++;
                std::filesystem::path folder = GetDataDir();
                stream.clear();
                stream << (const char*)"ScreenShot" << std::setfill('0') << std::setw(3) << counter << (const char*)".bmp";
                folder.append((const char*)"..");
                folder.append((const char*)stream.str().c_str());
                newFile = !std::filesystem::exists(folder.c_str());
                if (newFile)
                {
                    try
                    {
                        
                        //context.save_screenshot(folder);
                        ScreenShot(reinterpret_cast<const char*>(folder.c_str()));
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                    
                }
            }
#endif
            break;
        }
        break;
    }
  }
}

void Engine::render() {
  console.clear();
  map->render(console);
  for(auto const & actor : actors) {
    actor->render(console);
  }
  context.present(console);
}

void Engine::tryMove(Actor* actor, int x, int y) {
    if (x < 0 || x >= map->width || y < 0 || y >= map->height)
        return;

    if (map->isWall(x, y))
        return;
    
    actor->x = x;
    actor->y = y;
}

#ifndef __EMSCRIPTEN__
void Engine::ScreenShot(const char* fileName)
{
    int w, h;
    SDL_GetRendererOutputSize(context.get_sdl_renderer(), &w, &h);
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(context.get_sdl_renderer(), NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    SDL_SaveBMP(sshot, fileName);
    SDL_FreeSurface(sshot);
}
#endif


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
