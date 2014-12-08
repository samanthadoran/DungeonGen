#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <random>
#include <iostream>

#include "tile.h"

using std::vector;

class Room {
public:
    Room(int, int, int, int);

    void initRoom();

    bool intersectsRoom(Room) const;

    bool hasDoors() const;

    sf::Vector2i popDoor();

    int getX() const;

    int getY() const;

    sf::Vector2i getPos() const;

    int getWidth() const;

    int getLength() const;

    virtual ~Room();

protected:
private:
    int subtr;
    vector<sf::Vector2i> doors;
    int x;
    int y;
    int width;
    int length;
};

#endif // ROOM_H
