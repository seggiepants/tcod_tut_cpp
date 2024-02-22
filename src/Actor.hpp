#ifndef __ACTOR_HPP__
#define __ACTOR_HPP__

#include <fstream>
#include <iostream>
#include <libtcod.hpp>
#include <memory>
#include <string>

#include "Ai.hpp"
#include "Attacker.hpp"
#include "Destructible.hpp"

namespace Game {
class Confuser;
class Container;
class Fireball;
class Healer;
class LightningBolt;
class MonsterDestructible;
class Pickable;
class PlayerDestructible;

class Actor {
 public:
  int x, y;  // position on the map
  int ch;  // ascii code
  std::string name;
  TCOD_ColorRGBA col;  // color
  bool blocks;  // can we walk on this actor?
  Attacker* attacker;  // something that deals damage
  Destructible* destructible;  // something that can be damaged
  Ai* ai;  // something self-updating
  Pickable* pickable;  // something that can be picked and used.
  Container* container;  // something that can contain actors.
  Actor();
  Actor(int x, int y, int ch, const char* name, const TCOD_ColorRGBA& col);
  virtual ~Actor();
  void render() const;
  void update();
  void load(std::ifstream& stream);
  void save(std::ofstream& stream);
  float getDistance(int cx, int cy) const;
  bool fovOnly;
};
}  // namespace Game

#endif  //__ACTOR_HPP__
