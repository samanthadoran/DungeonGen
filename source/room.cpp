#include "../include/room.h"

Room::Room(int x, int y, int width, int length) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->length = length;
}

void Room::initRoom() {
    //Setup doors
    //int numDoors = (rand() % 2) + 1;
    int numDoors = 3;
    for (int i = 0; i < numDoors; ++i) {
        int workingX, workingY;
        int placementOne = rand() % 2;
        int placementTwo = rand() % 2;

        //Top/Bottom
        if (placementOne) {
            if (placementTwo)
                workingY = y;
            else
                workingY = y + length - 1;
            workingX = x + 1 + (rand() % (width - 2));
        }
        else {
            if (placementTwo)
                workingX = x;
            else
                workingX = x + width - 1;
            workingY = y + 1 + (rand() % (length - 2));
        }

        doors.push_back(sf::Vector2i(workingX, workingY));
    }
}

int Room::getX() const {
    return x;
}

int Room::getY() const {
    return y;
}

int Room::getWidth() const {
    return width;
}

int Room::getLength() const {
    return length;
}

bool Room::hasDoors() const {
    return doors.size();
}

sf::Vector2i Room::popDoor() {
    if (hasDoors()) {
        sf::Vector2i p = doors.back();
        doors.pop_back();
        return p;
    }
    return sf::Vector2i(0, 0);
}

bool Room::intersectsRoom(Room r) const {
    return (this->getX() <= r.getX() + r.getWidth()
            &&
            r.getX() <= this->getX() + this->getWidth()
                    &&
            this->getY() <= r.getY() + r.getLength()
                    &&
            r.getY() <= this->getY() + this->length
    );
}

Room::~Room() {
    //dtor
}
