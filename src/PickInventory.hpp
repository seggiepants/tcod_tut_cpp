#ifndef __PICKINVENTORY_HPP__
#define __PICKINVENTORY_HPP__

#include "Actor.hpp"
#include "Scene.hpp"

namespace Game {
class PickInventory : public Scene {
 public:
  PickInventory();
  void init();
  void destroy();
  Scene* update();
  void render();
  Actor* owner;
};
}  // namespace Game

#endif
