#ifndef ACTOR_H
#define ACTOR_H

class Item;

#include "entity.h"
#include "animatedsprite.hpp"
#include <vector>

using std::string;
using std::vector;

class Actor : public Entity, public sf::Drawable {
public:
    Actor(sf::Vector2f, int, string, string);

    Actor(const Actor &);

    virtual void control() = 0;

    virtual std::string toString() const = 0;

    virtual void act(Actor *) = 0;

    void update(sf::Time);

    double getHP() const;

    void setHP(double);

    double getDamage() const;

    vector<Actor *> getItems() const;

    Item *getSelectedItem();

    int getSelectedItemIndex() const;

    sf::RectangleShape getHealthBar() const;

    string getName() const;

    void addItem(Actor *);

    sf::Vector2f direction;

    int getPoints() const;

    void setPoints(int);

    void addPoints(int);

    virtual ~Actor();

private:
    Animation * walkingAnimationDown;
    Animation * walkingAnimationLeft;
    Animation * walkingAnimationRight;
    Animation * walkingAnimationUp;
    AnimatedSprite * animatedSprite;

    void draw(sf::RenderTarget &, sf::RenderStates) const;
    sf::RectangleShape healthBar;
    vector<Actor *> items;
    Item *selectedItem;
    int selectedItemIndex;
    string name;
    double damage;
    double hp;
    int points;
};

#endif