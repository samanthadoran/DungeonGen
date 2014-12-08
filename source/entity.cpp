#include "../include/entity.h"

Entity::Entity(int x, int y) {
    hp = 100;
    damage = 1;
    tex.loadFromFile("link.png");
    m_sprite.setTexture(tex);

    collision = sf::RectangleShape(sf::Vector2f(12, 8));
    collision.setPosition(position.x + 3, position.y + (26 - 8 - 1));

    position = sf::Vector2f(x, y);
    velocity = sf::Vector2f(0, 0);
}

Entity::Entity(sf::Vector2f pos) {
    hp = 100;
    damage = 1;
    tex.loadFromFile("link.png");
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

double Entity::getHP() const {
    return hp;
}

void Entity::setHP(double hp) {
    this->hp = hp;
}

double Entity::attack(Entity *e) {
    e->setHP(e->getHP() - this->getDamage());
    return this->getDamage();
}

double Entity::getDamage() const {
    return damage;
}

Entity::~Entity() {
    //dtor
}
