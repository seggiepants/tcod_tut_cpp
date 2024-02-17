#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__
#include <SDL.h>

#include "Engine.hpp"

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
  
}

/// Main program entry point.
int main(int argc, char** argv) {
  engine.preInit(argc, argv, 80, 50);
  engine.load();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
#else
    while (engine.isRunning()) 
    {
      main_loop();
    }
#endif
  engine.save();
  return 0;
}
