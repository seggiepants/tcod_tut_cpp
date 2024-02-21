#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__
#include <SDL.h>

#include "Engine.hpp"
#include "Game.hpp"

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif

Game::Engine engine;

/// Game loop.
void main_loop() {
  // Rendering.
  engine.update();
  engine.render();
  engine.present();
  if (engine.currentScene == nullptr) engine.Stop();
#ifdef __EMSCRIPTEN__
  if (!engine.isRunning()) {
    emscripten_cancel_main_loop();
  }
#endif
  
}

/// Main program entry point.
int main(int argc, char** argv) {
  engine.init(argc, argv, 80, 50);
  Game::Game* game = (Game::Game*)engine.scenes[Game::GameScene::GAME];
  game->load();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 30, 1);
#else
    while (engine.isRunning()) 
    {
      main_loop();
    }
#endif
  game->save();
  return 0;
}
