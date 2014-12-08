#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "equipment.h"


class Entity {
public:

    virtual void control() = 0;

    virtual std::string toString() const = 0;

    Entity(int, int);

    Entity(sf::Vector2f);

    void update(sf::Time);

    sf::Vector2f getPosition() const;

    void setPosition(sf::Vector2f);

    void setPosition(sf::Vector2i);

    sf::Sprite getSprite() const;

    sf::RectangleShape getCollisionAABB() const;

    sf::Vector2f getVelocity() const;

    void setVelocity(sf::Vector2f);

    double getHP() const;

    void setHP(double);

    double getDamage() const;

    //Returns change in HP of this argument
    double attack(Entity *);

    virtual ~Entity();

protected:
private:
    double damage;
    double hp;

    std::vector<Item> items;
    std::vector<Equipment> equipment;

    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Texture tex;
    sf::Sprite m_sprite;
    sf::RectangleShape collision;
};

#endif // ENTITY_H
