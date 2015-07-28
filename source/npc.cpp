#include "../include/npc.h"
NPC::NPC(sf::Vector2f pos, string filename) : Actor(pos, 0, filename, "Player") {
    target = nullptr;
    srand(time(0));
}

void NPC::setTarget(Actor *a) {
    target = a;
}

void NPC::act(Actor *a) {
    if (getItems().empty())
        return;
    Actor *item = getItems().back();
    item->act(a);
}

void NPC::control() {
    //Check distance
    if (target == nullptr)
        return;

    sf::Vector2f dist = getPosition() - target->getPosition();

    //Don't move about if the target is too far
    if (sqrt(dist.x * dist.x + dist.y * dist.y) > 300)
        return;

    if (sqrt(dist.x * dist.x + dist.y * dist.y) < 20) {
        act(target);
        return;
    }

    //Naively follow the player about
    double dir = atan2(getPosition().y - target->getPosition().y, getPosition().x - target->getPosition().x);

    double xVel = cos(dir) * -100;
    double yVel = sin(dir) * -100;

    setVelocity(sf::Vector2f(xVel, yVel));
}

std::string NPC::toString() const {
    std::stringstream toRet;
    toRet << "NPC" << std::endl;
    toRet << "\t At world position: (" << this->getPosition().x << ", " << this->getPosition().y << ")" << std::endl;
    toRet << "\t Has: " << this->getHP() << " hp";
    return toRet.str();
}

NPC::~NPC() {
    target = nullptr;
    //dtor
}
