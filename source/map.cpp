#include "../include/map.h"

Map::Map(int w, int h) {
    height = h;
    width = w;

    //Init and define adjacency matrix
    for (int j = 0; j < height; ++j) {
        adjacency.push_back(vector<vector<sf::Vector2i>>(width));
        for (int k = 0; k < width; ++k) {
            //Above
            if (j - 1 >= 0)
                adjacency[j][k].push_back(sf::Vector2i(k, j - 1));
            //Below
            if (j + 1 < height)
                adjacency[j][k].push_back(sf::Vector2i(k, j + 1));
            //Left
            if (k - 1 >= 0)
                adjacency[j][k].push_back(sf::Vector2i(k - 1, j));
            //Right
            if (k + 1 < width)
                adjacency[j][k].push_back(sf::Vector2i(k + 1, j));
        }
    }

    //I'm sure this can be done better.
    textures.push_back(new sf::Texture());
    textures.back()->loadFromFile("floor.png");
    textures.push_back(new sf::Texture());
    textures.back()->loadFromFile("wall.png");
    textures.push_back(new sf::Texture());
    textures.back()->loadFromFile("stairs_down.png");
    textures.push_back(new sf::Texture());
    textures.back()->loadFromFile("stairs_up.png");
}

//Consider removing in favour of finding where the stairs are on each floor change
void Map::initStairSpawns() {
    sf::Vector2i workingVec = stairsUpPos;
    for (sf::Vector2i p: adjacency[workingVec.y][workingVec.x])
        if (tiles[p.y][p.x]->getTileType() == TileType::FLOOR) {
            stairsUpSpawn = sf::Vector2i(p.y, p.x);
            break;
        }

    workingVec = stairsDownPos;
    for (sf::Vector2i p: adjacency[workingVec.y][workingVec.x])
        if (tiles[p.y][p.x]->getTileType() == TileType::FLOOR) {
            stairsDownSpawn = sf::Vector2i(p.y, p.x);
            break;
        }
}

void Map::addRow(string row) {
    int x = 0;
    int y = tiles.size();
    vector<Tile *> toAdd;
    for (char c: row) {
        if (Tile::getTileTypeFromText(c) == TileType::STAIRS_UP)
            stairsUpPos = sf::Vector2i(x, y);
        if (Tile::getTileTypeFromText(c) == TileType::STAIRS_DOWN)
            stairsDownPos = sf::Vector2i(x, y);
        toAdd.push_back(new Tile(&textures, y, x, Tile::getTileTypeFromText(c)));
        ++x;
    }
    tiles.push_back(toAdd);
}

string Map::getTextualRepresentation() const {
    stringstream s;
    for (auto row: tiles) {
        for (auto t: row)
            s << t->getTextualRepresentation();
        s << endl;
    }
    return s.str();
}

void Map::render() {
    cout << getTextualRepresentation();
}

//Tests whether or not a point lies within the bounds of the map.
bool Map::inBounds(sf::Vector2f test) const {
    return getTileAtPos(test) != nullptr;
}

vector<vector<Tile *>> Map::getMap() const {
    return tiles;
}

//What was I even doing?
bool Map::addRoom(int xPos, int yPos, int wid, int length) {
    if (getTileAtPos(sf::Vector2i(xPos, yPos)) == nullptr)
        return false;

    if (getTileAtPos(sf::Vector2i(xPos + wid, yPos + length)) == nullptr)
        return false;

    Room r(xPos, yPos, wid, length);
    for (Room rTest: rooms)
        if (r.intersectsRoom(rTest))
            return false;

    for (int j = 0; j < length; ++j) {
        if (j == 0 || j == length - 1) {
            for (int k = 0; k < wid; ++k) {
                tiles[j + yPos][k + xPos]->setTileType(TileType::WALL);
                filter[j + yPos][k + xPos] = 12;
            }
            continue;
        }

        for (int k = 0; k < wid; ++k) {
            if (k == 0 || k == wid - 1) {
                tiles[j + yPos][k + xPos]->setTileType(TileType::WALL);
                filter[j + yPos][k + xPos] = 12;
            }

            else {
                tiles[j + yPos][k + xPos]->setTileType(TileType::FLOOR);
                filter[j + yPos][k + xPos] = 8;
            }
        }
    }

    rooms.push_back(r);

    return true;
}

sf::Vector2i Map::minDist(vector<vector<int>> dst, vector<vector<bool>> visited) const {
    int minVal = -1;
    sf::Vector2i minPoint;

    //Find the min value of unvisited nodes
    for (int j = 0; j < height; ++j) {
        for (int k = 0; k < width; ++k) {
            //If the min value is infinity and our current distance is not infinity and has not been visited...
            //Set the minValue and nextNode
            if (minVal == -1 && dst[j][k] != -1 && !visited[j][k]) {
                minVal = dst[j][k];
                minPoint = sf::Vector2i(k, j);
            }
            else {
                //If we found a smaller distance, we haven't visited that point, and its distance is not infinity...
                //Set the minValue and nextNode
                if (dst[j][k] < minVal && !visited[j][k] && dst[j][k] != -1) {
                    minVal = dst[j][k];
                    minPoint = sf::Vector2i(k, j);
                }
            }
        }
    }
    return minPoint;
}

//Calculate the shortest path to every point from a source point
vector<vector<sf::Vector2i>> Map::djikstra(sf::Vector2i src) const {
    vector<vector<sf::Vector2i>> shortestPath;

    vector<vector<bool>> visited;

    vector<vector<int>> dst;

    //Init various vectors
    for (int j = 0; j < height; ++j) {
        shortestPath.push_back(vector<sf::Vector2i>(width));
        visited.push_back(vector<bool>(width, false));
        dst.push_back(vector<int>(width, -1));
    }

    //Mark source as visited
    visited[src.y][src.x] = true;
    //Min distance to source is 0 (from itself)
    dst[src.y][src.x] = 0;
    //Shortest path to source is itself
    shortestPath[src.y][src.x] = src;

    //Use a different node to preserve source, we'll need it later
    sf::Vector2i workingNode = src;
    for (int cnt = 0; cnt < width * height; ++cnt) {
        //Mark our current node as visited
        visited[workingNode.y][workingNode.x] = true;

        for (sf::Vector2i neighbour: adjacency[workingNode.y][workingNode.x]) {
            //Using the edge between workingNode and neighbour
            int testDist = dst[workingNode.y][workingNode.x] + filter[neighbour.y][neighbour.x];

            //Neighbour's current shortest distance
            int currDist = dst[neighbour.y][neighbour.x];

            //The distance is infinity, distance must be set with path
            if (currDist == -1) {
                currDist = testDist;
                dst[neighbour.y][neighbour.x] = testDist;
                shortestPath[neighbour.y][neighbour.x] = workingNode;
            }

            //The distance is lower, set it and the path
            if (testDist < currDist) {
                dst[neighbour.y][neighbour.x] = testDist;
                shortestPath[neighbour.y][neighbour.x] = workingNode;
            }
        }

        //Set the workingNode to be the unvisitedNode with lowestDst
        workingNode = minDist(dst, visited);
    }
    return shortestPath;
}

vector<sf::Vector2i> Map::generatePath(Tile *from, Tile *to) const {
    return generatePath(from->getPos(), to->getPos());
}

vector<sf::Vector2i> Map::generatePath(sf::Vector2i p1, sf::Vector2i p2) const {
    vector<vector<sf::Vector2i>> shortestPath = djikstra(p1);

    //Begin path recovery
    vector<sf::Vector2i> path;

    //Iterate backwards through shortestPath starting at p2
    sf::Vector2i workingNode;
    workingNode = p2;

    //While we haven't finished the path..
    while (workingNode != p1) {
        //Push back the next path chunk
        path.push_back(workingNode);

        //Continue iteration backwards
        workingNode = shortestPath[workingNode.y][workingNode.x];
    }

    //Add the beginning of the path
    path.push_back(p1);
    return path;

}

void Map::addPath(vector<sf::Vector2i> path) {
    for (sf::Vector2i p: path) {
        tiles[p.y][p.x]->setTileType(TileType::FLOOR);
        filter[p.y][p.x] += 2;
    }
}

void Map::generateMap() {
    //Fill with BLANK and init filter
    for (int j = 0; j < height; ++j) {
        tiles.push_back(vector<Tile *>(width));
        filter.push_back(vector<int>(width));
        for (int k = 0; k < width; ++k) {
            tiles[j][k] = new Tile(&textures, j, k);
            filter[j][k] = rand() % 8;
        }
    }

    //Generate Rooms
    int numRooms = ((rand() % 10) + 1) * 2;

    for (int i = 0; i < numRooms; ++i) {
        //Oh god, no.
        while (!addRoom(rand() % width, rand() % height, 5 + rand() % width / 5, 5 + rand() % height / 5));
    }

    for (Room &r: rooms)
        r.initRoom();

    do {
        //Rooms to take the doors from
        int uIndex = 0;
        int vIndex = 0;

        //Our doors
        sf::Vector2i u, v;

        int roomsFoundwDoors = 0;
        //Count the number of rooms with doors and set uIndex to be the last one.
        for (unsigned int i = 0; i < rooms.size(); ++i) {
            if (rooms[i].hasDoors()) {
                uIndex = i;
                roomsFoundwDoors++;
            }
        }

        //Something has gone wrong, a room doesn't have a partner.
        if (roomsFoundwDoors <= 1)
            break;

        //Get another room that has doors
        do {
            vIndex = rand() % rooms.size();
        }
        while (!rooms[vIndex].hasDoors());

        //Don't link the same room
        if (uIndex == vIndex)
            continue;

        u = rooms[uIndex].popDoor();
        v = rooms[vIndex].popDoor();

        //Make a corridor between u & v
        addPath(generatePath(u, v));
    }
    while (true);

    //Surround all floors with walls.
    for (vector<Tile *> row: tiles)
        for (Tile *t: row)
            if (t->getTileType() == TileType::FLOOR)
                //Try to place walls in blank spots adjacent to this tile
                for (auto point: adjacency[t->getPos().x][t->getPos().y])
                    if (tiles[point.y][point.x]->getTileType() == TileType::BLANK)
                        tiles[point.y][point.x]->setTileType(TileType::WALL);

    int stairsDownRoom;
    int stairsUpRoom;

    int x;
    int y;

    //Add stairs down
    stairsDownRoom = rand() % rooms.size();
    do {
        x = rand() % rooms[stairsDownRoom].getWidth() + rooms[stairsDownRoom].getX();
        y = rand() % rooms[stairsDownRoom].getLength() + rooms[stairsDownRoom].getY();
    }
    while (tiles[y][x]->getTileType() != TileType::FLOOR);

    tiles[y][x]->setTileType(TileType::STAIRS_DOWN);
    stairsDownPos = sf::Vector2i(x, y);

    //Add stairs up
    do {
        stairsUpRoom = rand() % rooms.size();
    }
    while (stairsUpRoom == stairsDownRoom);

    do {
        x = rand() % rooms[stairsUpRoom].getWidth() + rooms[stairsUpRoom].getX();
        y = rand() % rooms[stairsUpRoom].getLength() + rooms[stairsUpRoom].getY();
    }
    while (tiles[y][x]->getTileType() != TileType::FLOOR);

    tiles[y][x]->setTileType(TileType::STAIRS_UP);
    stairsUpPos = sf::Vector2i(x, y);

    initStairSpawns();

}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

sf::Vector2i Map::getStairsUpSpawn() const {
    return stairsUpSpawn;
}

sf::Vector2i Map::getStairsDownSpawn() const {
    return stairsDownSpawn;
}

//These are dealing with world coordinates
const Tile *const Map::getTileAtPos(sf::Vector2f input) const {
    int x = input.x / 32;
    int y = input.y / 32;

    //Remember to swap x & y
    return getTileAtPos(sf::Vector2i(y, x));
}

const Tile *const Map::getTileAtPos(sf::Vector2i input) const {
    //Check y bound
    if (input.y < 0 || input.y >= tiles.size())
        return nullptr;

    //Check x bound
    if (input.x < 0 || input.x >= tiles[0].size())
        return nullptr;

    return tiles[input.x][input.y];
}

Map::~Map() {
}