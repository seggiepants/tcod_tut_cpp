#ifndef __FIREBALL_HPP__
#define __FIREBALL_HPP__

#include <fstream>
#include <iostream>

#include "Pickable.hpp"

namespace Game {

class Actor;

class Fireball : public Pickable {
 public:
  float range;
  float damage;
  Fireball();
  Fireball(float range, float damage);
  bool use(Actor* owner, Actor* wearer);
  bool picked(Actor* owner, Actor* wearer, bool success, int x, int y);
  int getType() { return (int)PickableType::FIREBALL; };
  void load(std::ifstream& stream);
  void save(std::ofstream& stream);
};

}  // namespace Game

#endif
