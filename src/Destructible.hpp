#ifndef __DESTRUCTIBLE_HPP__
#define __DESTRUCTIBLE_HPP__

#include <string>

class Actor;
class Destructible {
public:
    Destructible(float maxHp, float defense, const char* corpseName);
    ~Destructible();
    inline bool isDead() { return hp <= 0; }
    float takeDamage(Actor* owner, float damage);
    virtual void die(Actor* owner);
    float maxHp; // maximum health points
    float hp; // current health points
    float defense; // hit points deflected
    std::string* corpseName; // the actor's name once dead/destroyed
};

#endif