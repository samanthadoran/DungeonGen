#include "../include/actor.h"

Actor::Actor(int x, int y, string filename, string name) : Entity(x, y, filename) {
    damage = 1;
    this->name = name;
    hp = 100;
}

Actor::Actor(sf::Vector2f pos, string filename, string name) : Entity(pos, filename) {
    damage = 1;
    this->name = name;
    hp = 100;
}

double Actor::getHP() const {
    return hp;
}

void Actor::setHP(double hp) {
    this->hp = hp;
}

double Actor::getDamage() const {
    return damage;
}

Actor::~Actor() {
    ;
}