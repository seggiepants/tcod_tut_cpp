#ifndef __MONSTERDESTRUCTIBLE_HPP__
#define __MONSTERDESTRUCTIBLE_HPP__

#include "Destructible.hpp"

namespace Game {
class MonsterDestructible : public Destructible {
 public:
  MonsterDestructible();
  MonsterDestructible(float maxHp, float defense, const char* corpseName, int xp);
  ~MonsterDestructible(){};
  int getType() { return (int)DestructibleType::MONSTER; }
  void die(Actor* owner);
  void save(std::ofstream& stream);
};
}  // namespace Game

#endif
