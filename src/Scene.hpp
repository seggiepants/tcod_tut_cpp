#ifndef __SCENE_HPP__
#define __SCENE_HPP__

namespace Game {
class Scene {
 public:
  virtual void init(){};
  virtual void destroy(){};
  virtual Scene* update() = 0;
  virtual void render() = 0;
};
}  // namespace Game

#endif
