#include "../include/player.h"

Player::Player() : Actor(sf::Vector2f(0, 0), 0, "link.png", "don'tuse") {
    ;
}

Player::Player(sf::Vector2f pos, int terminalVelocity, string filename) : Actor(pos, 0, filename, "Player") {
    this->terminalVelocity = terminalVelocity;
}

void Player::act(Actor *a) {
    Actor *item = getItems()[0];
    item->act(a);
}

void Player::control() {
    updateInput();
    parseInput();

    //Clamp velocity
    float magnitude = sqrt(getVelocity().x * getVelocity().x + getVelocity().y * getVelocity().y);
    sf::Vector2f unit = getVelocity()/magnitude;
    setVelocity(unit * terminalVelocity);
}

//May god forgive me for this function
void Player::updateInput() {
    input.clear();
    for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
        sf::Keyboard::Key k = static_cast<sf::Keyboard::Key>(i);
        if (sf::Keyboard::isKeyPressed(k))
            input.push_back(k);
    }
}

void Player::parseInput() {
    for (sf::Keyboard::Key key : input)
        control(key);
}

void Player::control(sf::Keyboard::Key key) {
    sf::Vector2f velocity = getVelocity();
    //Go up
    if (key == sf::Keyboard::W || key == sf::Keyboard::Up) {
        velocity += sf::Vector2f(0, -1 * terminalVelocity);
        direction.y = -1;
    }

    //Go down
    if (key == sf::Keyboard::S || key == sf::Keyboard::Down) {
        velocity += sf::Vector2f(0, terminalVelocity);
        direction.y = 1;
    }

    //Go left
    if (key == sf::Keyboard::A || key == sf::Keyboard::Left) {
        direction.x = -1;
        velocity += sf::Vector2f(-1 * terminalVelocity, 0);
    }

    //Go right
    if (key == sf::Keyboard::D || key == sf::Keyboard::Right) {
        direction.x = 1;
        velocity += sf::Vector2f(terminalVelocity, 0);
    }
    setVelocity(velocity);
}

std::string Player::toString() const {
    std::stringstream toRet;
    toRet << "Player" << std::endl;
    toRet << "\t At world position: (" << this->getPosition().x << ", " << this->getPosition().y << ")" << std::endl;
    toRet << "\t Has: " << this->getHP() << " hp";
    return toRet.str();
}

Player::~Player() {
    //dtor
}
