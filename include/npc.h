#ifndef NPC_H
#define NPC_H

#include <random>
#include <time.h>
#include "entity.h"

class NPC : public Entity {
public:
    NPC(int, int);

    void control();

    double attack(Entity *);

    std::string toString() const;

    NPC(sf::Vector2f);

    virtual ~NPC();

protected:
private:
};

#endif // NPC_H
