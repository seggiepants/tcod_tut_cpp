#ifndef __PLAYERAI_HPP__
#define __PLAYERAI_HPP__

#include <memory>

#include "Ai.hpp"

namespace Game {
class Actor;

class PlayerAi : public Ai {
 public:
  PlayerAi();
  ~PlayerAi(){};
  void update(Actor* owner);
  int getType() { return (int)AiType::PLAYER; };
  void useInventory(Actor* owner, Actor* wearer);
  void dropInventory(Actor* owner, Actor* wearer);
  void load(std::ifstream& stream);
  void save(std::ofstream& stream);
  int getNextLevelXp();
  int xpLevel;

 protected:
  bool moveOrAttack(Actor* owner, int targetX, int targetY);
  void handleActionKey(Actor* owner, int key);
  Actor* chooseFromInventory(Actor* owner);
};
}  // namespace Game

#endif
