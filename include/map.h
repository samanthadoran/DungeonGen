#ifndef MAP_H
#define MAP_H

#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <random>
#include <time.h>

#include "tile.h"
#include "room.h"


class Map : public sf::Drawable {
public:
    Map();

    Map(int, int);

    //For loading a map from a save
    void addRow(std::string);

    void initStairSpawns();

    void generateMap();

    void render();

    bool inBounds(sf::Vector2f) const;

    std::string getTextualRepresentation() const;

    int getWidth() const;

    int getHeight() const;

    vector<vector<Tile *>> *getMap();

    sf::Vector2i getStairsUpSpawn() const;

    sf::Vector2i getStairsDownSpawn() const;

    const Tile *const getTileAtPos(sf::Vector2f) const;

    const Tile *const getTileAtPos(sf::Vector2i) const;

    vector<sf::Vector2i> generatePath(Tile *, Tile *) const;

    virtual ~Map();

protected:
private:
    sf::Vector2i stairsUpPos;
    sf::Vector2i stairsDownPos;

    vector<sf::Texture *> textures;
    vector<vector<Tile *>> tiles;

    vector<vector<vector<sf::Vector2i>>> adjacency;
    vector<vector<int>> filter;

    sf::Vector2i minDist(vector<vector<int>>, vector<vector<bool>>) const;

    vector<vector<sf::Vector2i>> djikstra(sf::Vector2i) const;

    vector<sf::Vector2i> generatePath(sf::Vector2i, sf::Vector2i) const;

    void addPath(vector<sf::Vector2i>);

    //Please replace.
    bool addRoom(int, int, int, int);

    void draw(sf::RenderTarget &, sf::RenderStates) const;

    vector<Room> rooms;

    int width;
    int height;

    sf::Vector2i stairsUpSpawn;
    sf::Vector2i stairsDownSpawn;
};

#endif // MAP_H