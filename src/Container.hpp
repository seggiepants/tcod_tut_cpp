#ifndef __CONTAINER_HPP__
#define __CONTAINER_HPP__

#include <fstream>
#include <iostream>
#include <libtcod.hpp>
#include <list>
#include <memory>

namespace Game {

class Actor;

class Container {
 public:
  size_t size;
  std::list<Actor*> inventory;

  Container();
  Container(size_t size);
  virtual ~Container();
  bool add(Actor* actor);
  void remove(Actor* actor);
  void load(std::ifstream& stream);
  void save(std::ofstream& stream);
};
}  // namespace Game
#endif
