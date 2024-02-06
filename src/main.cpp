#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__
#include <SDL.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif

struct Player 
{
  int x;
  int y;
  char ch;
};

struct Player g_player;

/// Return the data directory.
auto get_data_dir() -> std::filesystem::path {
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

static constexpr auto WHITE = tcod::ColorRGB{255, 255, 255};

static tcod::Console g_console;  // The global console object.
static tcod::Context g_context;  // The global libtcod context.
bool running = true;

/// Game loop.
void main_loop() {
  // Rendering.
  g_console.clear();
  tcod::print(g_console, {0, 0}, "Hello World", WHITE, std::nullopt);
  g_console.at(g_player.x, g_player.y).ch = g_player.ch;
  g_context.present(g_console);

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
        switch(event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          running = false;
          break;
        case SDLK_UP:
        case SDLK_w:
          g_player.y--;
          break;
        case SDLK_DOWN:
        case SDLK_s:
          g_player.y++;
          break;
        case SDLK_LEFT:
        case SDLK_a:
          g_player.x--;
          break;
        case SDLK_RIGHT:
        case SDLK_d:
          g_player.x++;
          break;
        }
        // Bound to screen space.
        g_player.x = std::max(0, std::min(g_console.get_width() - 1, g_player.x));
        g_player.y = std::max(0, std::min(g_console.get_height() - 1, g_player.y));
        break;
    }
  }
}

/// Main program entry point.
int main(int argc, char** argv) {
  try {
    auto params = TCOD_ContextParams{};
    params.tcod_version = TCOD_COMPILEDVERSION;
    params.argc = argc;
    params.argv = argv;
    params.renderer_type = TCOD_RENDERER_SDL2;
    params.vsync = 1;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.window_title = "Libtcod C++ Tutorial";

    auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();

    g_console = tcod::Console{80, 40};
    params.console = g_console.get();

    g_context = tcod::Context(params);

    g_player.x = g_console.get_width() / 2;
    g_player.y = g_console.get_height() / 2;
    g_player.ch = '@';

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
#else
    while (running) 
    {
      main_loop();
    }
#endif
  } catch (const std::exception& exc) {
    std::cerr << exc.what() << "\n";
    throw;
  }
}
