#include "../include/npc.h"

NPC::NPC(int x, int y) : Actor(x, y) {
    srand(time(0));
}

NPC::NPC(sf::Vector2f pos) : Actor(pos) {
    srand(time(0));
}

void NPC::control() {
    sf::Vector2f testVel(rand() % 80 - 40, rand() % 80 - 40);
    setVelocity(testVel);
}

std::string NPC::toString() const {
    std::stringstream toRet;
    toRet << "NPC" << std::endl;
    toRet << "\t At world position: (" << this->getPosition().x << ", " << this->getPosition().y << ")" << std::endl;
    toRet << "\t Has: " << this->getHP() << " hp";
    return toRet.str();
}

NPC::~NPC() {
    //dtor
}
