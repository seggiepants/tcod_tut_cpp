#ifndef __PLAYERAI_HPP__
#define __PLAYERAI_HPP__

#include <memory>
#include "Ai.hpp"

namespace Game {
    class Actor;
    
    class PlayerAi : public Ai {
    public:
        ~PlayerAi() {};
        void update(Actor* owner);        
        int getType() {return (int)AiType::PLAYER; };
        void load(std::ifstream& stream);
        void save(std::ofstream& stream);
    protected:
        bool moveOrAttack(Actor* owner, int targetX, int targetY);
        void handleActionKey(Actor* owner, int key);
        Game::Actor* chooseFromInventory(Actor* owner);
    };
}

#endif