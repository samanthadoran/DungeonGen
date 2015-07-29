#ifndef DUNGEON_H
#define DUNGEON_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "map.h"

class Dungeon {
public:
    //Create a dungeon
    Dungeon();

    Dungeon(int, int);

    //Load a dungeon from a file
    Dungeon(std::string);

    const vector<Map *>& getFloors() const;

    Map *getFloor(int) const;

    void addFloor();

    void saveDungeon(std::string) const;

    virtual ~Dungeon();

protected:
private:
    int x, y;
    vector<Map *> floors;
};

#endif // DUNGEON_H