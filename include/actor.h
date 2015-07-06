#ifndef ACTOR_H
#define ACTOR_H

class Item;
#include "entity.h"
#include <vector>

using std::string;
using std::vector;

class Actor : public Entity {
public:
    Actor(int, int, int, string, string);

    Actor(sf::Vector2f, int, string, string);

    virtual void control() = 0;

    virtual std::string toString() const = 0;

    virtual void act(Actor *) = 0;

    void update(sf::Time);

    double getHP() const;

    void setHP(double);

    double getDamage() const;

    vector<Actor *> getItems() const;

    sf::RectangleShape getHealthBar() const;

    string getName() const;

    void addItem(Actor *);

    virtual ~Actor();

private:
    sf::RectangleShape healthBar;
    vector<Actor *> items;
    string name;
    double damage;
    double hp;
};

#endif