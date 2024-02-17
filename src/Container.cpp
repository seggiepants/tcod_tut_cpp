#include "Container.hpp"
#include "Engine.hpp"
#include "Actor.hpp"

Game::Container::Container() : size(0) {}

Game::Container::Container(size_t size) : size(size) {}

Game::Container::~Container() {
    for(auto& actor : inventory){
        actor.reset();
    }
    inventory.clear();
}

bool Game::Container::add(std::shared_ptr<Actor> actor) { 
    if (size > 0 && inventory.size() >= size) {
        // inventory full
        return false; 
    }
    inventory.push_back(actor);
    return true;
}

void Game::Container::remove(std::shared_ptr<Actor> actor) {
    inventory.remove(actor);
}

void Game::Container::load(std::ifstream& stream) {
    char delim = ',';
    int countItems = 0;
    inventory.clear();
    stream >> size >> delim >> countItems >> delim;
    for(int i = 0; i < countItems;++i) {
        std::shared_ptr<Game::Actor> item = std::make_shared<Game::Actor>();
        item->load(stream);
        inventory.push_back(item);
    }
}

void Game::Container::save(std::ofstream& stream) {
    const char delim = ',';
    stream << size << delim << inventory.size() << delim;
    
    for(auto & actor : inventory) {
        actor->save(stream);
    }
}