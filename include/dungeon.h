#ifndef DUNGEON_H
#define DUNGEON_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "map.h"

using namespace std;

class Dungeon {
public:
    //Create a dungeon
    Dungeon();

    Dungeon(int, int);

    //Load a dungeon from a file
    Dungeon(string);

    vector<Map> getFloors() const;

    Map getFloor(int) const;

    void addFloor();

    void saveDungeon(string) const;

    virtual ~Dungeon();

protected:
private:
    int x, y;
    vector<Map> floors;
};

#endif // DUNGEON_H