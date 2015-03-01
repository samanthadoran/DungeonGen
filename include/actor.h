#ifndef ACTOR_H
#define ACTOR_H

class Item;
#include "entity.h"
#include <vector>

using std::string;
using std::vector;

class Actor : public Entity {
public:
    Actor(int, int, string, string);

    Actor(sf::Vector2f, string, string);

    virtual void control() = 0;

    virtual std::string toString() const = 0;

    virtual void act(Actor *) = 0;

    double getHP() const;

    void setHP(double);

    double getDamage() const;

    vector<Actor *> getItems() const;

    string getName() const;

    void addItem(Actor *);

    virtual ~Actor();

private:
    vector<Actor *> items;
    string name;
    double damage;
    double hp;
};

#endif