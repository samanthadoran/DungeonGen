#include "../include/entity.h"

int Entity::nextUUID = -1;

Entity::Entity(int x, int y, string filename) {
    ++Entity::nextUUID;
    UUID = Entity::nextUUID;
    tex.loadFromFile(filename);
    m_sprite.setTexture(tex);

    collision = sf::RectangleShape(sf::Vector2f(12, 8));
    collision.setPosition(position.x + 3, position.y + (26 - 8 - 1));

    position = sf::Vector2f(x, y);
    velocity = sf::Vector2f(0, 0);
}

Entity::Entity(sf::Vector2f pos, string filename) {
    ++Entity::nextUUID;
    UUID = Entity::nextUUID;
    tex.loadFromFile(filename);
    m_sprite.setTexture(tex);

    collision = sf::RectangleShape(sf::Vector2f(12, 8));
    collision.setPosition(position.x + 3, position.y + (26 - 8 - 1));

    position = pos;
    velocity = sf::Vector2f(0, 0);
}

sf::RectangleShape Entity::getCollisionAABB() const {
    return collision;
}

void Entity::update(sf::Time elapsed) {
    position += velocity * elapsed.asSeconds();
    m_sprite.setPosition(position);
    collision.setPosition(position.x + 3, position.y + (26 - 8 - 1));
    velocity = sf::Vector2f(0, 0);
}

sf::Vector2f Entity::getPosition() const {
    return position;
}

sf::Sprite Entity::getSprite() const {
    return m_sprite;
}

sf::Vector2f Entity::getVelocity() const {
    return velocity;
}

unsigned int Entity::getUUID() const {
    return UUID;
}

void Entity::setVelocity(sf::Vector2f vel) {
    velocity = vel;
}

void Entity::setPosition(sf::Vector2f newPos) {
    position = newPos;
}

void Entity::setPosition(sf::Vector2i newPos) {
    int x = newPos.x;
    int y = newPos.y;
    position = sf::Vector2f(x, y);
}

Entity &Entity::operator=(const Entity &other) {
    tex = sf::Texture(*other.getSprite().getTexture());
    position = other.position;
    UUID = other.getUUID();
    collision = other.getCollisionAABB();
    return *this;
}

Entity::~Entity() {
    //dtor
}
