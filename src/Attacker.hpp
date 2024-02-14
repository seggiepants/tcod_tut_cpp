#ifndef __ATTACKER_HPP__
#define __ATTACKER_HPP__

namespace Game {
    class Actor;

    class Attacker {
        public:
        float power; // hit points given

        Attacker(float power);
        void attack(Actor* owner, Actor* target);
    };
}
#endif