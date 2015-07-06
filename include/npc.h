#ifndef NPC_H
#define NPC_H

#include <random>
#include <time.h>
#include "actor.h"
#include "map.h"

class NPC : public Actor {
public:
    NPC(sf::Vector2f, string);

    void act(Actor *);

    void control();

    void setTarget(Actor *);

    double attack(Entity *);

    std::string toString() const;

    virtual ~NPC();

protected:
private:
    Actor *target;
};

#endif // NPC_H
