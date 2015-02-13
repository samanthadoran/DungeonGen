#ifndef ACTOR_H
#define ACTOR_H

#include "entity.h"
#include "equipment.h"


class Actor : public Entity {
public:
    Actor(int, int, string);

    Actor(sf::Vector2f, string);

    virtual void control() = 0;

    virtual std::string toString() const = 0;

    double getHP() const;

    void setHP(double);

    double getDamage() const;

    //Returns change in HP of this argument
    double attack(Actor *);

    virtual ~Actor();

private:
    double damage;
    double hp;
    //std::vector<Item> items;
    //std::vector<Equipment> equipment;
};

#endif