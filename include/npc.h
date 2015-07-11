#ifndef NPC_H
#define NPC_H

#include <random>
#include <time.h>
#include "actor.h"
#include "map.h"

class NPC : public Actor {
public:
    NPC(sf::Vector2f, string);

    void act(Actor *) override;

    void control() override;

    void setTarget(Actor *);

    double attack(Entity *);

    std::string toString() const override;

    ~NPC();

protected:
private:
    Actor *target;
};

#endif // NPC_H
