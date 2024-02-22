#include "Pickable.hpp"

#include "Confuser.hpp"
#include "Container.hpp"
#include "Engine.hpp"
#include "Fireball.hpp"
#include "Healer.hpp"
#include "LightningBolt.hpp"

bool Game::Pickable::pick(Actor* owner, Actor* wearer) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  if (wearer->container && wearer->container->add(owner)) {
    game->actors.remove(owner);
    return true;
  }
  return false;
}
void Game::Pickable::drop(Actor* owner, Actor* wearer) {
  Game* game = (Game*)engine.scenes[GameScene::GAME];
  if (wearer->container) {
    wearer->container->remove(owner);

    game->actors.push_back(owner);
    engine.sendToBack(owner);

    owner->x = wearer->x;
    owner->y = wearer->y;
    game->gui->message(lightGrey, "%s drops a %s", wearer->name.c_str(), owner->name.c_str());
  }
}

Game::Pickable* Game::Pickable::create(int type) {
  Pickable* pickable = nullptr;
  switch ((PickableType)type) {
    case HEALER:
      pickable = new Healer(0);
      break;
    case LIGHTNING_BOLT:
      pickable = new LightningBolt(0, 0);
      break;
    case CONFUSER:
      pickable = new Confuser(0, 0);
      break;
    case FIREBALL:
      pickable = new Fireball(0, 0);
      break;
  }
  return pickable;
}
bool Game::Pickable::use(Actor* owner, Actor* wearer) {
  if (wearer->container) {
    wearer->container->remove(owner);
    delete owner;
    return true;
  }
  return false;
}
