#include "../include/actor.h"

Actor::Actor(sf::Vector2f pos, int damage, string filename, string name) : Entity(pos, filename) {
    healthBar = sf::RectangleShape(sf::Vector2f(24, 4));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setOrigin(healthBar.getSize() / 2.0f);
    healthBar.setPosition(getPosition());
    this->damage = damage;
    this->name = name;
    hp = 100;
}

void Actor::update(sf::Time elapsed) {
    Entity::update(elapsed);

    if (hp < 0)
        kill();

    healthBar.setPosition(getPosition() - sf::Vector2f(0, 13));
}

vector<Actor *> Actor::getItems() const {
    return items;
}

void Actor::addItem(Actor *a) {
    items.push_back(a);
}

string Actor::getName() const {
    return name;
}

double Actor::getHP() const {
    return hp;
}

sf::RectangleShape Actor::getHealthBar() const {
    return healthBar;
}

void Actor::setHP(double hp) {
    this->hp = hp;
    healthBar.setScale(sf::Vector2f(getHP() / 100.0, 1));
}

double Actor::getDamage() const {
    return damage;
}

Actor::~Actor() {
    for (int i = 0; i < items.size(); ++i) {
        delete items[i];
        items[i] = nullptr;
    }
}