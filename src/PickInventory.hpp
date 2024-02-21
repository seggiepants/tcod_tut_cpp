#ifndef __PICKINVENTORY_HPP__
#define __PICKINVENTORY_HPP__

#include "Scene.hpp"
#include "Actor.hpp"

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
