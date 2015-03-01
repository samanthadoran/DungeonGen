#include "../include/item.h"


Item::Item(sf::Vector2f pos, string filename, string name, int uses) : Actor(pos, filename, name) {
    //ctor
}

void Item::act(Actor *a) {
    //Update item uses
    setHP(getHP() - 1);

    a->setHP(a->getHP() - this->getDamage());
}


string Item::toString() const {
    return "" + getName() + " with " + "100" + " uses.";
}

void Item::control() {
    ;
}
Item::~Item() {
    //dtor
}
