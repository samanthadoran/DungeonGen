#include "../include/item.h"

Item::Item(sf::Vector2f pos, int damage, string filename, string name, int uses, int reUse) : Actor(pos, damage,
                                                                                                    filename, name) {
    setHP(uses);
    coolDown = reUse;
    setPoints(0);
    //ctor
}

Item::Item(const Item &other) : Actor(other) {
    coolDown = other.coolDown;
    setHP(other.getHP());
    setPoints(0);
}

void Item::act(Actor *a) {
    //Don't spam items!
    if (coolDownTimer.getElapsedTime().asMilliseconds() < coolDown)
        return;

    coolDownTimer.restart();

    if (getHP() <= 0 && getHP() != -1)
        return;

    if (getHP() != -1)
        setHP(getHP() - 1);

    if (a != nullptr)
        a->setHP(a->getHP() - this->getDamage());
}

Item *Item::pickUp() {
    //Grab a copy of our item
    Item *toRet = new Item(*this);

    //Kill this instance, it is being picked up
    kill();

    //Give the item away
    return toRet;
}

string Item::toString() const {
    std::stringstream toRet;
    toRet << getName() << " with " << getHP() << " uses.";
    return toRet.str();
}

void Item::control() {
    ;
}

void Item::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(getSprite(), states);
}

Item::~Item() {
    //dtor
}