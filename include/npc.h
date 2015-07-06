#ifndef NPC_H
#define NPC_H

#include <random>
#include <time.h>
#include "actor.h"
#include "map.h"

class NPC : public Actor {
public:
    Actor *target;

    NPC(int, int, string, Actor *);

    NPC(sf::Vector2f, string, Actor *);

    void act(Actor *);

    void control();

    double attack(Entity *);

    std::string toString() const;

    virtual ~NPC();

protected:
private:
};

#endif // NPC_H
