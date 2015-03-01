#ifndef ITEM_H
#define ITEM_H

#include "actor.h"

#include <string>

using std::string;

class Item : public Actor {
public:
    Item(sf::Vector2f, string, string, int);

    void act(Actor *);

    virtual ~Item();

protected:
private:
};

#endif // ITEM_H
