#ifndef __LIGHTNINGBOLT_HPP__
#define __LIGHTNINGBOLT_HPP__

#include "Pickable.hpp"

namespace Game {
class Actor;

class LightningBolt : public Pickable {
 public:
  float range;
  float damage;
  LightningBolt();
  LightningBolt(float range, float damage);
  bool use(Actor* owner, Actor* wearer);
  int getType() { return (int)PickableType::LIGHTNING_BOLT; };
  void load(std::ifstream& stream);
  void save(std::ofstream& stream);
  template <class Archive>
  void serialize(Archive& ar) {
#ifndef __EMSCRIPTEN__
    ar(range, damage);
#endif
  }
};
}  // namespace Game

#endif
