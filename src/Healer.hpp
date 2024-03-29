#ifndef __HEALER_HPP__
#define __HEALER_HPP__

#include "Pickable.hpp"

namespace Game {

class Actor;

class Healer : public Pickable {
 public:
  float amount;  // how many hp
  Healer();
  Healer(float amount);
  bool use(Actor* owner, Actor* wearer);
  int getType() { return (int)PickableType::HEALER; };
  void load(std::ifstream& stream);
  void save(std::ofstream& stream);
};
}  // namespace Game

#endif
