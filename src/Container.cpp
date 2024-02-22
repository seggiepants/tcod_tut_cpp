#include "Container.hpp"

#include "Actor.hpp"
#include "Engine.hpp"

Game::Container::Container() : size(0) {}

Game::Container::Container(size_t size) : size(size) {}

Game::Container::~Container() {
  for (auto& actor : inventory) {
    delete actor;
  }
  inventory.clear();
}

bool Game::Container::add(Actor* actor) {
  if (size > 0 && inventory.size() >= size) {
    // inventory full
    return false;
  }
  inventory.push_back(actor);
  return true;
}

void Game::Container::remove(Actor* actor) { inventory.remove(actor); }

void Game::Container::load(std::ifstream& stream) {
  char delim = ',';
  int countItems = 0;
  inventory.clear();
  stream >> size >> delim >> countItems >> delim;
  for (int i = 0; i < countItems; ++i) {
    Actor* item = new Actor();
    item->load(stream);
    inventory.push_back(item);
  }
}

void Game::Container::save(std::ofstream& stream) {
  const char delim = ',';
  stream << size << delim << inventory.size() << delim;

  for (auto& actor : inventory) {
    actor->save(stream);
  }
}
