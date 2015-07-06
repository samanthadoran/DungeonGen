#include "../include/dungeon.h"

//Please don't use this.
Dungeon::Dungeon() {
    ;
}

Dungeon::Dungeon(int width, int height) {
    x = width;
    y = height;
    floors.emplace_back(new Map(x, y));
    floors.back()->generateMap();
}

//Load from file...
Dungeon::Dungeon(string filename) {
    ifstream ifp(filename);
    cout << "Loading..." << endl;
    if (!ifp.is_open())
        return;

    int numFloors;
    int height;
    int width;

    ifp >> height;
    ifp >> width;
    ifp >> numFloors;

    x = width;
    y = height;

    //Iterate over all specified floors
    for (int j = 0; j < numFloors; ++j) {
        floors.push_back(new Map(height, width));
        string row;

        //Iterate over the specified rows
        for (int k = 0; k < height; ++k) {
            //Get a string representation of the row of tiles
            ifp >> row;

            //Send this string to Map for parsing
            floors[j]->addRow(row);
        }
        //When loading from a file, this must be explicitly called!
        floors[j]->initStairSpawns();
    }
    //Clean up
    ifp.close();
}

//Return all of the floors
vector<Map *> Dungeon::getFloors() const {
    return floors;
}

//Return a specific floor
Map *Dungeon::getFloor(int index) const {
    return floors[index];
}

//Add a floor and generate its map
void Dungeon::addFloor() {
    floors.push_back(new Map(x, y));
    floors.back()->generateMap();
}

//Save to file...
void Dungeon::saveDungeon(string filename) const {
    ofstream ofp(filename);

    if (!ofp.is_open())
        return;

    //Empty Map
    if (floors[0]->getHeight() == 0 || floors[0]->getWidth() == 0)
        return;

    //Setup the header
    ofp << floors[0]->getHeight() << endl;
    ofp << floors[0]->getWidth() << endl;
    ofp << floors.size() << endl;

    //Quick and easy method to save
    for (auto f: floors)
        ofp << f->getTextualRepresentation();

    //Clean up
    ofp.close();
}

Dungeon::~Dungeon() {
    for (int i = 0; i < floors.size(); ++i) {
        delete floors[i];
        floors[i] = nullptr;
    }
    floors.clear();
}