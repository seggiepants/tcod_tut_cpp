#ifndef __DESTRUCTIBLE_HPP__
#define __DESTRUCTIBLE_HPP__

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace Game {
class Actor;

class Destructible {
 public:
  Destructible();
  Destructible(float maxHp, float defense, const char* corpseName, int xp);
  virtual ~Destructible();
  virtual int getType() = 0;
  inline bool isDead() { return hp <= 0; }
  float heal(float amount);
  float takeDamage(Actor* owner, float damage);
  virtual void die(Actor* owner);
  template <class Archive>
  void serialize(Archive& ar) {
    ar(maxHp, hp, defense, corpseName);
  }
  float maxHp;  // maximum health points
  float hp;  // current health points
  float defense;  // hit points deflected
  int xp; // XP gained when killing this monster (or player xp)
  std::string corpseName;  // the actor's name once dead/destroyed
  void load(std::ifstream& stream);
  void save(std::ofstream& stream);
  enum DestructibleType { MONSTER, PLAYER };
  static Destructible* create(int type);
};
}  // namespace Game

#endif
