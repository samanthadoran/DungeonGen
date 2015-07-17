#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"
#include <iostream>

using namespace std;

using std::vector;

class Player : public Actor {
public:
    Player();

    Player(sf::Vector2f, int, string);

    void act(Actor *) override;

    std::string toString() const override;

    void control() override;

    ~Player();

protected:
private:
    void updateInput();

    void parseInput();

    vector<sf::Keyboard::Key> input;

    void control(sf::Keyboard::Key);

    float terminalVelocity;
};

#endif // PLAYER_H
