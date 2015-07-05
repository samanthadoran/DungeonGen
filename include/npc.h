#ifndef NPC_H
#define NPC_H

#include <random>
#include <time.h>
#include "actor.h"
#include "map.h"

class NPC : public Actor {
public:
    Actor *target;

    NPC(int, int, string, Map *);

    NPC(sf::Vector2f, string, Map *);

    void act(Actor *);

    void control();

    double attack(Entity *);

    std::string toString() const;

    virtual ~NPC();

protected:
private:
    Map *m;
};

#endif // NPC_H
