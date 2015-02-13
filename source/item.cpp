#include "../include/item.h"


Item::Item(sf::Vector2f pos, string filename, string name, int uses) : Entity(pos, filename) {
    this->name = name;
    this->uses = uses;
    //ctor
}

Item::~Item() {
    //dtor
}
