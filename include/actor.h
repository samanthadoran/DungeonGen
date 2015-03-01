#ifndef ACTOR_H
#define ACTOR_H

#include "entity.h"

using std::string;

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

    virtual ~Actor();

private:
    string name;
    double damage;
    double hp;
};

#endif