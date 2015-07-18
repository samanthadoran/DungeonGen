#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

using std::string;

class Entity {
public:

    virtual std::string toString() const = 0;

    Entity(sf::Vector2f, string);

    Entity(const Entity &);

    void update(sf::Time);

    sf::Vector2f getPosition() const;

    void setPosition(sf::Vector2f);

    void setPosition(sf::Vector2i);

    void kill();

    bool isAlive() const;

    unsigned int getUUID() const;

    const sf::Sprite &getSprite() const;

    sf::RectangleShape getCollisionAABB() const;

    sf::Vector2f getVelocity() const;

    void setVelocity(sf::Vector2f);

    Entity &operator=(const Entity &);

    virtual ~Entity();

protected:
private:
    unsigned int UUID;
    static int nextUUID;

    bool alive;

    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Texture tex;
    sf::Sprite m_sprite;
    sf::RectangleShape collision;
};

#endif // ENTITY_H
