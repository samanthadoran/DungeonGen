#ifndef ITEM_H
#define ITEM_H

#include "actor.h"

#include <string>

using std::string;

class Item : public Actor {
public:
    Item(sf::Vector2f, string, string, int, int);

    string toString() const;

    void control();

    void act(Actor *);

    virtual ~Item();

protected:
private:
    double coolDown;
    sf::Clock coolDownTimer;
};

#endif // ITEM_H