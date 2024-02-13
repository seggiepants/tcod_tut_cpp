#include "Container.hpp"

Game::Container::Container(size_t size) : size(size) {}

Game::Container::~Container() {
    for(auto const & actor : inventory){
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

void Game::Container::remove(Actor* actor) {
    inventory.remove(actor);
}
