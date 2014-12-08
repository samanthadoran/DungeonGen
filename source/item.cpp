#include "../include/item.h"

Item::Item(string name, int uses) {
    this->name = name;
    this->uses = uses;
    //ctor
}

/*void Item::use(Entity * e)
{
    e->setHP(e->getHP() + 0);
}*/

Item::~Item() {
    //dtor
}
