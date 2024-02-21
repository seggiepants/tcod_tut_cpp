#ifndef __DESTRUCTIBLE_HPP__
#define __DESTRUCTIBLE_HPP__

#include <iostream>
#include <fstream>
#include <memory>
#include <string>


namespace Game {
    class Actor;
    
    class Destructible {
    public:
        Destructible();
        Destructible(float maxHp, float defense, const char* corpseName);
        virtual ~Destructible();
        virtual int getType() = 0;
        inline bool isDead() { return hp <= 0; }
        float heal(float amount);
        float takeDamage(Actor* owner, float damage);
        virtual void die(Actor* owner);
        template <class Archive> void serialize(Archive& ar) {
            ar(maxHp, hp, defense, corpseName);
        }
        float maxHp; // maximum health points
        float hp; // current health points
        float defense; // hit points deflected
        std::string corpseName; // the actor's name once dead/destroyed
        void load(std::ifstream& stream);
        void save(std::ofstream& stream);
        enum DestructibleType {
            MONSTER,PLAYER
        };
        static Destructible* create(int type);
    };
}

#endif
