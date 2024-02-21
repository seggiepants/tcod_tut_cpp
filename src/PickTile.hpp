#ifndef __PICKTILE_HPP__
#define __PICKTILE_HPP__

#include "Scene.hpp"

namespace Game {
  class PickTile : public Scene {
 public:
    PickTile();
  void init();
  void destroy();
  Scene* update();
  void render();
  float maxRange;

  protected:
  bool isPositionOK(int x, int y);
  bool mouseClicked;
  int mouseX;  // mouse position in tiles/characters
  int mouseY;
  int currentKey;
  int x;
  int y;
  };
}

#endif
