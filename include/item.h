#ifndef ITEM_H
#define ITEM_H

#include "actor.h"

#include <string>

using std::string;

class Item : public Actor {
public:
    Item(sf::Vector2f, int, string, string, int, int);

    Item(const Item &);

    string toString() const override;

    void control() override;

    void act(Actor *) override;

    Item *pickUp();

    virtual ~Item();

protected:
private:
    void draw(sf::RenderTarget &, sf::RenderStates) const;

    double coolDown;
    sf::Clock coolDownTimer;
};

#endif // ITEM_H