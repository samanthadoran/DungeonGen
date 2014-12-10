#include "../include/actor.h"

Actor::Actor(int x, int y, string filename) : Entity(x, y, filename) {
    damage = 1;
    hp = 100;
}

Actor::Actor(sf::Vector2f pos, string filename) : Entity(pos, filename) {
    damage = 1;
    hp = 100;
}


double Actor::getHP() const {
    return hp;
}

void Actor::setHP(double hp) {
    this->hp = hp;
}

double Actor::attack(Actor *a) {
    a->setHP(a->getHP() - this->getDamage());
    return this->getDamage();
}

double Actor::getDamage() const {
    return damage;
}

Actor::~Actor() {
    ;
}