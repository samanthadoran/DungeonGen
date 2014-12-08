#include "../include/actor.h"

Actor::Actor(int x, int y) : Entity(x, y) {

}

Actor::Actor(sf::Vector2f pos) : Entity(pos) {

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