#include "../include/actor.h"

Actor::Actor(int x, int y, string filename, string name) : Entity(x, y, filename) {
    healthBar = sf::RectangleShape(sf::Vector2f(24, 4));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(getPosition());
    damage = 1;
    this->name = name;
    hp = 100;
}

Actor::Actor(sf::Vector2f pos, string filename, string name) : Entity(pos, filename) {
    healthBar = sf::RectangleShape(sf::Vector2f(24, 4));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(getPosition());
    damage = 1;
    this->name = name;
    hp = 100;
}

void Actor::update(sf::Time elapsed) {
    Entity::update(elapsed);
    healthBar.setPosition(getPosition());
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
    ;
}