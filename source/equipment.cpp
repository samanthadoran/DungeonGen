#include "../include/equipment.h"

Equipment::Equipment(sf::Vector2f pos, string filename, string name) : Item(pos, 0, filename, name, 42, 0) {
    //ctor
}

Equipment::~Equipment() {
    //dtor
}
