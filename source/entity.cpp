#include "../include/entity.h"

int Entity::nextUUID = -1;

Entity::Entity(sf::Vector2f pos, string filename) {
    alive = true;
    ++Entity::nextUUID;
    UUID = Entity::nextUUID;
    tex.loadFromFile(filename);
    m_sprite.setTexture(tex);
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);

    collision = sf::RectangleShape(sf::Vector2f(12, 8));
    collision.setOrigin(collision.getLocalBounds().width / 2.0f, collision.getLocalBounds().height / 2.0f);
    collision.setPosition(pos);

    collision = sf::RectangleShape(sf::Vector2f(12, 8));
    collision.setPosition(pos);

    position = pos;
    velocity = sf::Vector2f(0, 0);
}

sf::RectangleShape Entity::getCollisionAABB() const {
    return collision;
}

void Entity::kill() {
    alive = false;
}

bool Entity::isAlive() const {
    return alive;
}

void Entity::update(sf::Time elapsed) {
    position += velocity * elapsed.asSeconds();
    m_sprite.setPosition(position);
    collision.setPosition(position);
    velocity = sf::Vector2f(0, 0);
}

sf::Vector2f Entity::getPosition() const {
    return position;
}

const sf::Sprite &Entity::getSprite() const {
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
    velocity = other.getVelocity();
    UUID = other.getUUID();
    collision = other.getCollisionAABB();
    return *this;
}

Entity::~Entity() {
    //dtor
}
